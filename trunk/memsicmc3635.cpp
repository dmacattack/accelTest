#include "memsicmc3635.hpp"
#include <QDebug>
#include <QThread>

// anonymous namespace
namespace
{
    const int INIT_REGISTER_ADDR = 0x0F;  // see mc3635 data sheet register 0x0F
    enum eINIT_REG_VALUE
    {
        eINIT_STARTUP = 0x45,          // initial startup sequence
        eINIT_STARTUP_COMPLETE = 0x40, // startup sequence complete
        eINIT_SW_START = 0x42,         // to be written by software on startup
        eINIT_COMPLETE = 0x43          // init complete after SW start
    };
}

/**
 * @brief MemsicMC3635::MemsicMC3635 - ctor
 * @param i2cBus - i2c bus number
 * @param i2cAddr - i2c address
 */
MemsicMC3635::MemsicMC3635(int i2cBus, int i2cAddr)
: UIUSensorBase(i2cBus, i2cAddr)
{

}

/**
 * @brief MemsicMC3635::initDevice - reset & init the memsic mc3635 sensor for reading uiu angles
 */
void MemsicMC3635::initDevice()
{
    // reset the device to ensure the correct settings
    reset();
    // short sleep after reset
    QThread::msleep(10);
    // init the device to i2c
    setI2CFeature();
    // init reg1
    init1();
    // reset the drive motion axis registers
    initDriveMotionRegisters();
    // init the other two init registers
    init23();
    // set the range(optional, but helps with accuracy)
    setRange(MEMSIC::eRES_12BITS, MEMSIC::eRANGE_2G);
    // set the mode
    setMode(MEMSIC::eMODE_SNIFF);
}

/**
 * @brief MemsicMC3635::reset - reset the sensor.
 * @note !!!  Minimum 1 ms delay should follow this call before the sensor is ready !!!
 */
void MemsicMC3635::reset()
{
    // put device in standby
    setMode(MEMSIC::eMODE_STANDBY);
    // reset
    MEMSIC::tRegResetRegister resetControl;
    resetControl.reset = 1;
    writeByte(MEMSIC::eREG_RESET, resetControl.value);
}

/**
 * @brief MemsicMC3635::init1 - perform the init process on init register 1
 */
void MemsicMC3635::init1()
{
    int initReg = readByte(MEMSIC::eREG_INIT1);
    switch (initReg)
    {
    case eINIT_STARTUP:
        qWarning() << "sensor init not complete. wait and try again";
        break;
    case eINIT_STARTUP_COMPLETE:
        // request the init
        writeByte(MEMSIC::eREG_INIT1, eINIT_SW_START);

        // readback
        if (readByte(MEMSIC::eREG_INIT1) != eINIT_COMPLETE)
        {
            qWarning() << "sw startup not complete. readback = " << hex << initReg;
        }
        break;
    case eINIT_SW_START:
    case eINIT_COMPLETE:
    default:
        // do nothing, should be okay
        break;
    }
}

/**
 * @brief MemsicMC3635::init23 - perform the init process on init registers 2 & 3
 */
void MemsicMC3635::init23()
{
    writeByte(MEMSIC::eREG_INIT2, 0x00);
    writeByte(MEMSIC::eREG_INIT3, 0x00);
}

/**
 * @brief MemsicMC3635::initDriveMotionRegisters - init the Drive Motion Registers
 */
void MemsicMC3635::initDriveMotionRegisters()
{
    // disable all the drive motion registers
    qDebug() << "init dmx";
    writeByte(MEMSIC::eREG_DMX, 0x01);
    qDebug() << "init dmy";
    writeByte(MEMSIC::eREG_DMY, 0x80);
    qDebug() << "init dmz";
    writeByte(MEMSIC::eREG_DMZ, 0x00);
}

/**
 * @brief MemsicMC3635::setI2CFeature - set the sensor to i2c mode
 */
void MemsicMC3635::setI2CFeature()
{
    MEMSIC::tRegFeatureRegister1 i2cFeature;
    i2cFeature.i2cEn = 1;
    writeByte(MEMSIC::eREG_FEATURE1, i2cFeature.value);
}

/**
 * @brief MemsicMC3635::setMode - set the mode of the sensor
 */
void MemsicMC3635::setMode(MEMSIC::eACCELEROMETER_MODE mode)
{
    MEMSIC::tRegModeControl modeControl;
    modeControl.mode = mode;
    writeByte(MEMSIC::eREG_MODEC, modeControl.value);
}

/**
 * @brief MemsicMC3635::setRange - set the resolution and range of the sensor
 */
void MemsicMC3635::setRange(MEMSIC::eACCELEROMETER_RESOLUTION resolution, MEMSIC::eACCELEROMETER_RANGE range)
{
    MEMSIC::tRegRangeControl rangeControl;
    rangeControl.resolution = resolution;
    rangeControl.range = range;
    writeByte(MEMSIC::eREG_RANGE_C, rangeControl.value);
}

/**
 * @brief MemsicMC3635::getStatus1 - read the status register 1 from the sensor
 */
MEMSIC::tRegStatusRegister1 MemsicMC3635::getStatus1()
{
    MEMSIC::tRegStatusRegister1 status;
    status.value = readByte(MEMSIC::eREG_STATUS1);
    return status;
}

/**
 * @brief MemsicMC3635::getStatus2 - read the status register 2 from the sensor
 */
MEMSIC::tRegStatusRegister2 MemsicMC3635::getStatus2()
{
    MEMSIC::tRegStatusRegister2 status;
    status.value = readByte(MEMSIC::eREG_STATUS2);
    return status;
}

/**
 * @brief MemsicMC3635::getMode - read the mode control register from the sensor
 */
MEMSIC::tRegModeControl MemsicMC3635::getMode()
{
    MEMSIC::tRegModeControl mode;
    mode.value = readByte(MEMSIC::eREG_MODEC);
    return mode;
}

/**
 * @brief MemsicMC3635::getRange - read the range from the sensor
 */
MEMSIC::tRegRangeControl MemsicMC3635::getRange()
{
    MEMSIC::tRegRangeControl range;
    range.value = readByte(MEMSIC::eREG_RANGE_C);
    return range;
}

/**
 * @brief MemsicMC3635::getXSensor - provide the x sensor reading
 */
INSP_UIU_SENSOR::tAxisReading MemsicMC3635::getXSensor()
{
    return getSensor(MEMSIC::eREG_XOUT_LSB, MEMSIC::eREG_XOUT_MSB);
}

/**
 * @brief MemsicMC3635::getYSensor - provide the y sensor reading
 */
INSP_UIU_SENSOR::tAxisReading MemsicMC3635::getYSensor()
{
    return getSensor(MEMSIC::eREG_YOUT_LSB, MEMSIC::eREG_YOUT_MSB);
}

/**
 * @brief MemsicMC3635::getZSensor - provide the z sensor reading
 */
INSP_UIU_SENSOR::tAxisReading MemsicMC3635::getZSensor()
{
    return getSensor(MEMSIC::eREG_ZOUT_LSB, MEMSIC::eREG_ZOUT_MSB);
}

/**
 * @brief MemsicMC3635::getSensor - provide a sensor reading from the provided LSB/MSB registers
 */
INSP_UIU_SENSOR::tAxisReading MemsicMC3635::getSensor(MEMSIC::eREGISTERS lsbReg, MEMSIC::eREGISTERS msbReg)
{
    INSP_UIU_SENSOR::tAxisReading reading;
    reading.i2cResp = INSP_I2C_DEVICE::eERROR_NONE;

    // read the lsb/msb
    int lsb = readByte(lsbReg);
    int msb = readByte(msbReg);
    if (lsb < 0)
    {
        reading.i2cResp = lsb;
    }
    else if (msb < 0)
    {
        reading.i2cResp = msb;
    }
    else
    {
        // add the accelerometer data if no i2c errors
        MEMSIC::tRegAxis axis;
        axis.lsb = lsb;
        axis.msb = msb;
        reading.axisData = axis.value;
    }

    return reading;
}
