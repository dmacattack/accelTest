#include <QCoreApplication>
#include <QDebug>
#include "uiucontroller.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "create i2c accelerometer";

    UIUController controller(INSP_UIU::eSENSOR_MEMSIC_MC3635, 1);

    QObject::connect(&controller, &UIUController::updateAngle, [&](double angle)
    {
        qDebug() << angle;
    });

    QObject::connect(&controller, &UIUController::updateI2CError, [&](int i2cErr)
    {
        qDebug() << "i2c error" << InspI2CDevice::getErrString(i2cErr);
    });

    controller.start();


    return a.exec();
}
