#include "memsicmc3635.hpp"

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
 * @param i2cbus - i2c bus number
 * @param i2cAddr - i2c address
 */
MemsicMC3635::MemsicMC3635(int i2cbus, int i2cAddr)
: InspI2CDevice(i2cbus, i2cAddr)
{

}

/**
 * @brief MemsicMC3635::initDevice - set the mode to i2c and init the device
 */
void MemsicMC3635::initDevice()
{
    // host must set the mode to i2c
    MEMSIC::tRegFeatureRegister1 i2cFeature;
    i2cFeature.i2cEn = 1;
    writeByte(MEMSIC::eREG_FEATURE1, i2cFeature.value);

    // host must write in 0x42 upon power on
    writeByte(MEMSIC::eREG_INIT1, eINIT_SW_START);

    // host must write in 0x00
    writeByte(MEMSIC::eREG_INIT3, 0x00);

    // host must write in 0x00
    writeByte(MEMSIC::eREG_INIT2, 0x00);

    // set the rate ?
}


