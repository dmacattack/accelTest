#ifndef NXPMMA8652_HPP
#define NXPMMA8652_HPP

#include "uiusensorbase.hpp"

namespace NXP_MMA8652
{
    // registers
    enum eREGISTERS
    {
        eREG_WHO_AM_I  = 0x0d,
        eREG_F_SETUP   = 0x09,
        eREG_F_MODE    = 0x2a,
        eREG_CTRL_REG1 = 0x2a,
        eREG_X_MSB     = 0x01,
        eREG_X_LSB     = 0x02,
        eREG_Y_MSB     = 0x03,
        eREG_Y_LSB     = 0x04,
        eREG_Z_MSB     = 0x05,
        eREG_Z_LSB     = 0x06
    };

    // axis check to ensure the data is refreshed correctly
    struct tSensorAxisStatus
    {
        bool xStatusReady;
        bool yStatusReady;
        bool zStatusReady;

        // default ctor
        tSensorAxisStatus()
        {
            xStatusReady = false;
            yStatusReady = false;
            zStatusReady = false;
        }
    };

    // fifo setup register 0x09
    union tRegFifoSetup
    {
        unsigned char value;
        struct
        {
            unsigned char fifoWatermark : 6; // b0-5
            unsigned char fifoMode      : 2; // b6-7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            QString modeStr = (fifoMode == 0x00   ? "FIFO disabled"                  :
                              (fifoMode == 0x01   ? "FIFO circ buffer - discard old" :
                              (fifoMode == 0x10   ? "FIFO circ buffer - discard new" :
                              (fifoMode == 0x11   ? "FIFO trigger mode"              : "???" ))));

            return QString("register: = 0x%1 \n"
                           "   mode        : %2 \n"
                           "   watermark   : %3 ")
                    .arg(value, 1, 16)
                    .arg(modeStr)
                    .arg(fifoWatermark);
        }
    };
}

/**
 * @brief The NXPMMA8652 class - object to provide access to the NXP MMA8625 sensor
 */
class NXPMMA8652
: public UIUSensorBase
{
public:
    /**
     * @brief NXPMMA8652 - ctor
     * @param i2cBus - i2c bus number
     * @param i2cAddr - i2c address
     */
    NXPMMA8652(int i2cBus, int i2cAddr);

    /**
     * @brief ~NXPMMA8652 - dtor
     */
    ~NXPMMA8652() {}

    /**
     * @brief initDevice - reset & init the nxp mma8652 sensor for reading uiu angles
     */
    bool initDevice();

    /**
     * @brief getXSensor - provide the x sensor reading
     */
    INSP_UIU_SENSOR::tAxisReading getXSensor();

    /**
     * @brief getYSensor - provide the y sensor reading
     */
    INSP_UIU_SENSOR::tAxisReading getYSensor();

    /**
     * @brief getZSensor - provide the z sensor reading
     */
    INSP_UIU_SENSOR::tAxisReading getZSensor();


private:
    /**
     * @brief get8BitSensor - provide an 8bit sensor reading from the provided LSB/MSB registers
     */
    INSP_UIU_SENSOR::tAxisReading get8BitSensor(NXP_MMA8652::eREGISTERS msbReg);

    /**
     * @brief isMMA8652Device - read the who_am_i register to ensure this is the correct device
     */
    bool isMMA8652Device();

    /**
     * @brief twosComplimentToDecimal - convert 2s compliment register to decimal
     * @param gValue - gsensor 2s compliment value (12bit or 8bit register)
     * @returns decimal value
     */
    qint16 twosComplimentToDecimal(qint16 gValue);

private:
    NXP_MMA8652::tSensorAxisStatus mSensorStatus;
    INSP_UIU_SENSOR::tAxisReading mXAxis;
    INSP_UIU_SENSOR::tAxisReading mYAxis;
    INSP_UIU_SENSOR::tAxisReading mZAxis;

};

#endif // NXPMMA8652_HPP
