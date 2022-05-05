#ifndef UIUSENSORBASE_HPP
#define UIUSENSORBASE_HPP

#include <QObject>
#include "include/inspi2cdevice.hpp"

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
    virtual void initDevice() = 0;

    /**
     * @brief getXSensor - provide the x sensor reading
     */
    virtual int getXSensor() = 0;

    /**
     * @brief getYSensor - provide the y sensor reading
     */
    virtual int getYSensor() = 0;

    /**
     * @brief getZSensor - provide the z sensor reading
     */
    virtual int getZSensor() = 0;
};

#endif // UIUSENSORBASE_HPP
