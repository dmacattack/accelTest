#ifndef MEMSICMC3635_HPP
#define MEMSICMC3635_HPP

#include "memsicregisters.hpp"
#include "uiusensorbase.hpp"

/**
 * @brief The MemsicMC3635 class - object to provide access to the Memsic MC3635 sensor
 */
class MemsicMC3635
: public UIUSensorBase
{
public:
    /**
     * @brief MemsicMC3635 - ctor
     * @param i2cBus - i2c bus number
     * @param i2cAddr - i2c address
     */
    MemsicMC3635(int i2cBus, int i2cAddr);

    /**
     * @brief ~MemsicMC3635 - dtor
     */
    ~MemsicMC3635() {}

    /**
     * @brief initDevice - reset & init the memsic mc3635 sensor for reading uiu angles
     */
    bool initDevice();

    /**
     * @brief reset - reset the sensor.
     * @note !!!  Minimum 1 ms delay should follow this call before the sensor is ready !!!
     */
    void reset();

    /**
     * @brief init1 - perform the init process on init register 1
     */
    void init1();
    /**
     * @brief init23 - perform the init process on init registers 2 & 3
     */
    void init23();

    /**
     * @brief initDriveMotionRegisters - init the Drive Motion Registers
     */
    void initDriveMotionRegisters();

    /**
     * @brief setI2CFeature - set the sensor to i2c mode
     */
    void setI2CFeature();

    /**
     * @brief setMode - set the mode of the sensor
     */
    void setMode(MEMSIC::eACCELEROMETER_MODE mode);

    /**
     * @brief setRange - set the resolution and range of the sensor
     */
    void setRange(MEMSIC::eACCELEROMETER_RESOLUTION resolution, MEMSIC::eACCELEROMETER_RANGE range);

    /**
     * @brief getStatus1 - read the status register 1 from the sensor
     */
    MEMSIC::tRegStatusRegister1 getStatus1();

    /**
     * @brief getStatus2 - read the status register 2 from the sensor
     */
    MEMSIC::tRegStatusRegister2 getStatus2();

    /**
     * @brief getMode - read the mode control register from the sensor
     */
    MEMSIC::tRegModeControl getMode();

    /**
     * @brief getRange - read the range from the sensor
     */
    MEMSIC::tRegRangeControl getRange();

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
     * @brief getSensor - provide a sensor reading from the provided LSB/MSB registers
     */
    INSP_UIU_SENSOR::tAxisReading getSensor(MEMSIC::eREGISTERS lsbReg, MEMSIC::eREGISTERS msbReg);

};


#endif // MEMSICMC3635_HPP
