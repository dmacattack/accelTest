#ifndef MEMSICMC3635_HPP
#define MEMSICMC3635_HPP

#include <QObject>
#include "memsicregisters.hpp"

class MemsicMC3635
{
public:
    MemsicMC3635();

private:

    void initDevice();
    void i2cWrite(unsigned char addr, unsigned char val);
    int i2cRead(unsigned char addr);

};


#endif // MEMSICMC3635_HPP
