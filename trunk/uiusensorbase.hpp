#ifndef UIUSENSORBASE_HPP
#define UIUSENSORBASE_HPP

#include <QObject>
#include "include/inspi2cdevice.hpp"

namespace INSP_UIU_SENSOR
{
    /*
     * Accelerometer axis data is often 16 bit, 2s complement. To ensure i2c errors can be captured
     * This data type allows the sensor readings to capture the 16bit data & i2c errors.
     */
    union tAxisReading
    {
        int value;
        struct
        {
            short i2cResp;
            short axisData;
        };

        // default ctor
        tAxisReading()
        {
            clear();
        }

        // clear function
        void clear()
        {
            set(INSP_I2C_DEVICE::eERROR_NONE, 0);
        }

        // setter function
        void set(short i2cResp, short axisData)
        {
            this->i2cResp = i2cResp;
            this->axisData = axisData;
        }
    };
}

/**
 * @brief The UIUSensorBase class - base class for Up-Is-Up sensors
 */
class UIUSensorBase
: public InspI2CDevice
{
public:
    /**
     * @brief UIUSensorBase - ctor
     * @param i2cbus - i2c bus number
     * @param i2cAddr - i2c address
     */
    UIUSensorBase(int i2cBus, int i2cAddr);

    /**
     * @brief UIUSensorBase - dtor
     */
    virtual ~UIUSensorBase() {}

public:
    /**
     * @brief initDevice - sensor specific initialization
     */
    virtual bool initDevice() = 0;

    /**
     * @brief getXSensor - provide the x sensor reading
     */
    virtual INSP_UIU_SENSOR::tAxisReading getXSensor() = 0;

    /**
     * @brief getYSensor - provide the y sensor reading
     */
    virtual INSP_UIU_SENSOR::tAxisReading getYSensor() = 0;

    /**
     * @brief getZSensor - provide the z sensor reading
     */
    virtual INSP_UIU_SENSOR::tAxisReading getZSensor() = 0;
};

#endif // UIUSENSORBASE_HPP
