#ifndef MEMSICMC3635_HPP
#define MEMSICMC3635_HPP

#include <QObject>
#include "memsicregisters.hpp"
#include "include/inspi2cdevice.hpp"

class MemsicMC3635
: public InspI2CDevice
{
public:
    MemsicMC3635(int i2cbus, int i2cAddr);

private:

    void initDevice();

};


#endif // MEMSICMC3635_HPP
