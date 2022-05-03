#include <QCoreApplication>
#include "memsicmc3635.hpp"
#include <QDebug>

#include "include/inspgpio.hpp"
#include "include/inspi2cdevice.hpp"

#include <QTimer>

void testGPIOLib()
{
    qDebug() << "setup gpio 230";
    InspGPIO gpio230("gpio7_b6", INSP_GPIO::eDIRECTION_OUT, INSP_GPIO::eEDGE_BOTH, false, 0);
    InspGPIO gpio231("gpio7_b7", INSP_GPIO::eDIRECTION_OUT, INSP_GPIO::eEDGE_BOTH, false, 0);

    int cnt = 0;

    QTimer *pTimer= new QTimer();

    QObject::connect(pTimer, &QTimer::timeout, [&]()
    {

        int rb0 = gpio230.read();
        int rb1 = gpio231.read();

        qDebug() << "gpio readback = " << rb0 << rb1;

        int val0 = 0;
        int val1 = 0;

        switch (cnt)
        {
        case 0:
            val0 = 0;
            val1 = 0;
            break;
        case 1:
            val0 = 0;
            val1 = 1;

            break;
        case 2:
            val0 = 1;
            val1 = 0;
            break;
        case 3:

            val0 = 1;
            val1 = 1;
            break;
        default:
            break;
        }

        qDebug() << "===============";
        qDebug() << "seting gpios to " << val0 << val1 ;

        gpio230.write(val0);
        gpio231.write(val1);


        cnt++;
        if (cnt > 3)
        {
            cnt = 0;
        }
    });

    pTimer->start(2000);
}



void testI2clib()
{

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const int SCRATCH_REG = 0x1b;

    QTimer *pTimer = new QTimer();
    InspI2CDevice i2c(1, 0x4c);

    QObject::connect(pTimer, &QTimer::timeout, [&]()
    {
        qDebug() << "try to read from the register";
        int scratch = i2c.readByte(SCRATCH_REG);
        qDebug() << "scratch register = " << scratch;

        scratch += 2;
        if (scratch == 255)
        {
            scratch = 0;
        }

        i2c.writeByte(SCRATCH_REG, scratch);
    });

    pTimer->start(2000);

    return a.exec();
}
