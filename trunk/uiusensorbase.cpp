#include "uiusensorbase.hpp"

/**
 * @brief UIUSensorBase::UIUSensorBase - ctor
 * @param i2cbus - i2c bus number
 * @param i2cAddr - i2c address
 */
UIUSensorBase::UIUSensorBase(int i2cBus, int i2cAddr)
: InspI2CDevice(i2cBus, i2cAddr)
{

}
