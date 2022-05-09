#include "nxpmma8652.hpp"
#include <QDebug>
//the MMA8652 supports 8 or 12 bits of data, were using 8 bits for now
//but the code should support 12 bits no problem
#define USE_12_BIT_DATA 0
#define GSENSOR_DEBUG 0

// anonymous namespace
namespace
{
    // mma8652 i2c register values
    const int MMA8652_WHO_AM_I_ID             = 0x4A;
    const int MMA8652_F_MODE_BITSHIFT         = 6; //bits 6-7
    const int MMA8652_CIRCULAR_BUFFER_SETTING = (1 << MMA8652_F_MODE_BITSHIFT);
    const int MMA8652_ACTIVE_BIT              = 0x01;
    const int MMA8652_FAST_READ_MODE          = 0x02;
}

/**
 * @brief NXPMMA8652::NXPMMA8652 - ctor
 * @param i2cBus - i2c bus number
 * @param i2cAddr - i2c address
 */
NXPMMA8652::NXPMMA8652(int i2cBus, int i2cAddr)
: UIUSensorBase(i2cBus, i2cAddr)
, mSensorStatus()
, mXAxis()
, mYAxis()
, mZAxis()
{

}

/**
 * @brief NXPMMA8652::initDevice - reset & init the nxp mma8652 sensor for reading uiu angles
 */
bool NXPMMA8652::initDevice()
{
    bool isInit = true;
    if (!isMMA8652Device())
    {
        isInit = false;
        int deviceId = readByte(NXP_MMA8652::eREG_WHO_AM_I);
        qWarning() << "device is not the correct one. Expected " << MMA8652_WHO_AM_I_ID << "got" << deviceId;
    }
    else
    {
#if USE_12_BIT_DATA
        // set the fifo mode to a circular buffer
        writeByte(NXP_MMA8652::eREG_F_SETUP, MMA8652_CIRCULAR_BUFFER_SETTING);
#else
        // set the fast mode read bit. this disables auto address incrementing
        // but only offers us 8 bits of resolution instead of 12
        writeByte(NXP_MMA8652::eREG_CTRL_REG1, MMA8652_FAST_READ_MODE);
#endif
        // set the chip to active
        writeByte(NXP_MMA8652::eREG_CTRL_REG1, MMA8652_ACTIVE_BIT);

        qDebug() << "Accelerometer ID is Good";
    }

    return isInit;
}

/**
 * @brief NXPMMA8652::getXSensor - provide the x sensor reading
 */
INSP_UIU_SENSOR::tAxisReading NXPMMA8652::getXSensor()
{
    // the NXP sensor requires reading all the axises in sequence
    mXAxis.clear();
    mYAxis.clear();
    mZAxis.clear();

#if USE_12_BIT_DATA
    // TODO not reliable. should be revisited

    qint16 y_msb = 0;
    qint16 z_msb = 0;
    qint16 x_msb=0
    qint16 x_lsb=0;
    qint16 y_lsb=0;
    qint16 z_lsb=0;
    qint16 z_gvalue=0;
    qint16 y_gvalue=0;

    char buf[6] = {0};
    memset(buf, 0, NUM_OF_REGISTERS_TO_READ);

    if(m_i2cBus.i2c_read_continuous(mI2cAddr, MMA8652_X_MSB, buf, NUM_OF_REGISTERS_TO_READ)
            != NUM_OF_REGISTERS_TO_READ)
    {
        qWarning() << "Could not read all axis values from accelerometer";
        return;
    }

    x_msb = buf[0];
    x_lsb = buf[1];
    y_msb = buf[2];
    y_lsb = buf[3];
    z_msb = buf[4];
    z_lsb = buf[5];

    Q_UNUSED(x_msb);
    Q_UNUSED(x_lsb);

    // combine the upper 4 bits of the lsb register with the 8 bits of the msb register
    // to get the full 12 bits of resoultion
    // The datasheet says its MSB<11:4>LSB<3:0 - 0000>, but looking at the data the MSB & LSB were backwards,
    // so they're OR'd in reverse order here
    z_gvalue = (z_lsb << 4) | (z_msb >> 4);
    y_gvalue = (y_lsb << 4) | (y_msb >> 4);

#else

    // read the values one at a time, only grabbing the 8 most signif. bits
    mXAxis = get8BitSensor(NXP_MMA8652::eREG_X_MSB);
    mYAxis = get8BitSensor(NXP_MMA8652::eREG_Y_MSB);
    mZAxis = get8BitSensor(NXP_MMA8652::eREG_Z_MSB);

#endif

    // update the sensor flags to ensure data is always fresh
    mSensorStatus.xStatusReady = false;
    mSensorStatus.yStatusReady = true;
    mSensorStatus.zStatusReady = true;

    return mXAxis;
}

/**
 * @brief NXPMMA8652::getYSensor - provide the y sensor reading
 */
INSP_UIU_SENSOR::tAxisReading NXPMMA8652::getYSensor()
{
    if (mSensorStatus.yStatusReady == false)
    {
        qWarning() << "refreshing sensors since x was not read yet"; // TODO remove
        // refresh the sensors
        getXSensor();
    }

    // close the y Sensor flag
    mSensorStatus.yStatusReady = false;

    return mYAxis;
}

/**
 * @brief NXPMMA8652::getZSensor - provide the z sensor reading
 */
INSP_UIU_SENSOR::tAxisReading NXPMMA8652::getZSensor()
{
    if (mSensorStatus.zStatusReady == false)
    {
        qWarning() << "refreshing sensors since x was not read yet"; // TODO remove
        // refresh the sensors
        getXSensor();
    }

    // close the z Sensor flag
    mSensorStatus.zStatusReady = false;

    return mZAxis;
}

/**
 * @brief get8BitSensor - provide an 8bit sensor reading from the provided LSB/MSB registers
 */
INSP_UIU_SENSOR::tAxisReading NXPMMA8652::get8BitSensor(NXP_MMA8652::eREGISTERS msbReg)
{
    INSP_UIU_SENSOR::tAxisReading axisReading;

    qint16 msb = readByte(msbReg);
    if (msb < 0)
    {
        // i2c error, fill the error
        axisReading.i2cResp = msb;
    }
    else
    {
        // no error. add data
        axisReading.axisData = twosComplimentToDecimal(msb);
    }

    return axisReading;
}

/**
 * @brief NXPMMA8652::isMMA8652Device - read the who_am_i register to ensure this is the correct device
 */
bool NXPMMA8652::isMMA8652Device()
{
    int ret = readByte(NXP_MMA8652::eREG_WHO_AM_I);
    return (ret == MMA8652_WHO_AM_I_ID);
}

/**
 * @brief NXPMMA8652::twosComplimentToDecimal - convert 2s compliment register to decimal
 * @param gValue - gsensor 2s compliment value (12bit or 8bit register)
 * @returns decimal value
 */
qint16 NXPMMA8652::twosComplimentToDecimal(qint16 gValue)
{
    qint16 decimal_gvalue=0;
    quint8 bitShift = 7; //we have 8 bits, so shifting left 7 brings us to the 8th bit
#if USE_12_BIT_DATA
    bitShift = 11; //12 bits of data, same thing
#endif
    bool isNegative = false;

    //these values are twos complement, we need to do sign extension on the 8 bits
    isNegative = (gValue & (1 << bitShift)) != 0;

    // If the number is considered negative (i.e. bit 7 or 11 is set),
    // we bitwise-or it with a bit pattern that has ones in all the remaining bits.
    // This creates the proper negative native-sized integer.
    if(isNegative)
    {
        decimal_gvalue = gValue | ~((1 << bitShift) -1);
    }
    else
    {
        decimal_gvalue = gValue;
    }

    return decimal_gvalue;
}

