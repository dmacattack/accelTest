#ifndef UIUCONTROLLER_HPP
#define UIUCONTROLLER_HPP

#include <QObject>
#include "uiusensorbase.hpp"
#include <QTimer>

namespace INSP_UIU
{
    enum eSENSOR_TYPE
    {
        eSENSOR_NONE = -1,     // no UIU sensor type
        eSENSOR_CFAST,         // sensor data is available through a cfast connector
        eSENSOR_NXP_MMA8652,   // NXPs MMCA8652 3 axis accerometer
        eSENSOR_MEMSIC_MC3635  // Memsic MC3635 3 axis accelerometer
    };

    const int DEFAULT_POLL_INTERVAL = 100;  // 100ms
}

/**
 * @brief The UIUController class - object to implement UIU across multiple sensor types
 */
class UIUController
: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief UIUController - ctor
     * @param sensor - sensor type
     * @param i2cBus - i2c bus number
     * @param i2cAddr - i2c address
     * @param pollMs(optional) - poll interval for the sensor. default = DEFAULT_POLL_INTERVAL
     */
    UIUController(INSP_UIU::eSENSOR_TYPE sensor, int i2cBus, int i2cAddr, int pollMs = INSP_UIU::DEFAULT_POLL_INTERVAL);

    /**
     * @brief UIUController - dtor
     */
    ~UIUController();

    /**
     * @brief setPollInterval - set the polling interval for angle updates
     * @param ms - time in milliseconds
     */
    void setPollInterval(int ms);

    /**
     * @brief start - start acquiring samples
     */
    void start();

    /**
     * @brief stop - stop acquiring samples
     */
    void stop();

signals:
    /**
     * @brief updateAngle - signal to provide the updated angle
     */
    void updateAngle(double angle);

private slots:
    /**
     * @brief onUpdateAngle - timer slot callback to read the newest angle from the sensor
     */
    void onUpdateAngle();

private:
    /**
     * @brief setSensorType - set the sensor type
     * @param sensor - sensor type
     * @param i2cBus - i2c bus number
     * @param i2cAddr - i2c address
     */
    void setSensorType(INSP_UIU::eSENSOR_TYPE sensor, int i2cBus, int i2cAddr);

private:
    UIUSensorBase *mpSensor;
    QTimer *mpPollTimer;


};

#endif // UIUCONTROLLER_HPP
