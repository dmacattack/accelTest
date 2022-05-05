#include "uiucontroller.hpp"
#include <QDebug>
#include "memsicmc3635.hpp"
#include <QtMath>

// anonymous namespace
namespace
{
    const double _180_OVER_PI        = 57.29578;
}

/**
 * @brief UIUController - ctor
 * @param sensor - sensor type
 * @param i2cBus - i2c bus number
 * @param i2cAddr - i2c address
 * @param pollMs(optional) - poll interval for the sensor. default = DEFAULT_POLL_INTERVAL
 */
UIUController::UIUController(INSP_UIU::eSENSOR_TYPE sensor, int i2cBus, int i2cAddr, int pollMs)
: mpSensor(NULL)
, mpPollTimer(new QTimer())
{
    // set sensor details
    setSensorType(sensor, i2cBus, i2cAddr);

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
void UIUController::start()
{
    // init the sensor
    mpSensor->initDevice();

    // start the poll timer
    mpPollTimer->start();
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
void UIUController::setSensorType(INSP_UIU::eSENSOR_TYPE sensor, int i2cBus, int i2cAddr)
{
    if (sensor == INSP_UIU::eSENSOR_MEMSIC_MC3635)
    {
        mpSensor = new MemsicMC3635(i2cBus, i2cAddr);
    }
    else
    {
        qFatal("sensor not currently supported");
    }
}
