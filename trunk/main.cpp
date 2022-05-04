#include <QCoreApplication>
#include "memsicmc3635.hpp"
#include <QDebug>

#include "include/inspgpio.hpp"
#include "include/inspi2cdevice.hpp"

#include <QTimer>
#include <QThread>

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
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "create i2c accelerometer";
    // create the accelerometer
    InspI2CDevice accel(1, 0x4c);

    qDebug() << "init device to i2c, not spi";
    // init the device to i2c
    MEMSIC::tRegFeatureRegister1 i2cFeature;
    i2cFeature.i2cEn = 1;
    accel.writeByte(MEMSIC::eREG_FEATURE1, i2cFeature.value);

    qDebug() << "do the three register init";
    int initReg = accel.readByte(MEMSIC::eREG_INIT1);
    qDebug() << "  original init reg " << hex << initReg;
    accel.writeByte(MEMSIC::eREG_INIT1, 0x42);
    // read the reg again after a short period
    QThread::msleep(50);
    initReg = accel.readByte(MEMSIC::eREG_INIT1);
    qDebug() << "  init reg after init " << hex << initReg;

    qDebug() << "  init the other inits to 0";
    accel.writeByte(MEMSIC::eREG_INIT2, 0x00);
    accel.writeByte(MEMSIC::eREG_INIT3, 0x00);

    qDebug() << "try to read some registers";

    for (int i = 0 ; i < 50; i++)
    {
        MEMSIC::tRegAxis x;
        x.lsb = accel.readByte(MEMSIC::eREG_XOUT_LSB);
        x.msb = accel.readByte(MEMSIC::eREG_XOUT_MSB);

        qDebug() << "x axis = " << x.value;

        QThread::msleep(100);

    }




    return a.exec();
}
