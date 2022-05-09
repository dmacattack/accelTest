#include "uiucontroller.hpp"
#include <QDebug>
#include <QtMath>
#include "memsicmc3635.hpp"
#include "nxpmma8652.hpp"

// anonymous namespace
namespace
{
    const quint8 MMA8652_I2C_ADDRESS = 0x1d;
    const quint8 MC3535_I2C_ADDRESS  = 0x4c;
    const double _180_OVER_PI        = 57.29578;
}

/**
 * @brief UIUController - ctor
 * @param sensor - sensor type
 * @param i2cBus - i2c bus number
 * @param pollMs(optional) - poll interval for the sensor. default = DEFAULT_POLL_INTERVAL
 */
UIUController::UIUController(INSP_UIU::eSENSOR_TYPE sensor, int i2cBus, int pollMs)
: mpSensor(NULL)
, mpPollTimer(new QTimer())
{
    // set sensor details
    setSensorType(sensor, i2cBus);

    // set poll interval
    setPollInterval(pollMs);

    // connect timer
    QObject::connect(mpPollTimer, &QTimer::timeout, this, &UIUController::onUpdateAngle);
}

/**
 * @brief UIUController - dtor
 */
UIUController::~UIUController()
{
    delete mpPollTimer;
    delete mpSensor;
}

/**
 * @brief setPollInterval - set the polling interval for angle updates
 * @param ms - time in milliseconds
 */
void UIUController::setPollInterval(int ms)
{
    mpPollTimer->setInterval(ms);
}

/**
 * @brief start - start acquiring samples
 */
bool UIUController::start()
{
    // init the sensor
    bool init = mpSensor->initDevice();

    if (init)
    {
        // start the poll timer
        mpPollTimer->start();
    }

    return init;
}

/**
 * @brief stop - stop acquiring samples
 */
void UIUController::stop()
{
    // stop the poll timer
    mpPollTimer->stop();
}

/**
 * @brief onUpdateAngle - timer slot callback to read the newest angle from the sensor
 */
void UIUController::onUpdateAngle()
{
    // read y & z axis
    auto y = mpSensor->getYSensor();
    auto z = mpSensor->getZSensor();

    if (y.i2cResp != INSP_I2C_DEVICE::eERROR_NONE  || z.i2cResp != INSP_I2C_DEVICE::eERROR_NONE)
    {
        // most likely z axis has the same error
        emit updateI2CError(y.i2cResp);
    }
    else
    {
        // convert to degrees
        double angle = atan2(y.axisData, z.axisData) * _180_OVER_PI;

        // send out update
        emit updateAngle(angle);
    }
}

/**
 * @brief setSensorType - set the sensor type
 * @param sensor - sensor type
 */
void UIUController::setSensorType(INSP_UIU::eSENSOR_TYPE sensor, int i2cBus)
{
    if (sensor == INSP_UIU::eSENSOR_MEMSIC_MC3635)
    {
        mpSensor = new MemsicMC3635(i2cBus, MC3535_I2C_ADDRESS);
    }
    else if (sensor == INSP_UIU::eSENSOR_NXP_MMA8652)
    {
        mpSensor = new NXPMMA8652(i2cBus, MMA8652_I2C_ADDRESS);
    }
    else
    {
        qFatal("sensor not currently supported");
    }
}
