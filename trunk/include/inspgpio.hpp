#ifndef INSPGPIO_HPP
#define INSPGPIO_HPP

#include <QString>

namespace INSP_GPIO
{
    enum eGPIO_DIRECTION
    {
        eDIRECTION_OUT = 0,
        eDIRECTION_IN = 1,
    };

    enum eGPIO_EDGE
    {
        eEDGE_BOTH        = 0,
        eEDGE_FALLING     = 1,
        eEDGE_RISING      = 2,
        eEDGE_NONE        = 3
    };
}

/**
 * @brief The InspGPIO class - provide userspace access to GPIOs
 * TODO this is a placeholder
 */
class InspGPIO
{
public:
    /**
     * @brief InspGPIO - ctor
     * @param gpioNum - linux userspace gpio number
     * @param direction - gpio direction
     * @param edge - gpio edge property
     * @param activeLow - gpio activity
     * @param initialValue - initial GPIO value
     */
    InspGPIO(int gpioNum, INSP_GPIO::eGPIO_DIRECTION direction, INSP_GPIO::eGPIO_EDGE edge, bool activeLow, int initialValue);

    /**
     * @brief InspGPIO - overloaded ctor with rockchip gpioName
     * @param rkGpioName - rockchip gpio name
     * @param direction - gpio direction
     * @param edge - gpio edge property
     * @param activeLow - gpio activity
     * @param initialValue - initial GPIO value
     */
    InspGPIO(const QString &rkGpioName, INSP_GPIO::eGPIO_DIRECTION direction, INSP_GPIO::eGPIO_EDGE edge, bool activeLow, int initialValue);

    /**
     * @brief read - read the gpio value
     */
    int read();

    /**
     * @brief write - write a value to the gpio
     * @param val - value to write (1 or 0)
     */
    void write(int val);

    /**
     * @brief rockchipGPIOToNumber - convert a rockchip gpio name to a linux userspace number
     * This implements the equation:
     *
     * gpio<block>_<bank><num> : [ 32*block + (bank-1)*8 ] + num - 8
     *
     * @param gpioName (case insensitive) - the name of the gpio as shown in schematic. "gpio7_b8"
     * @returns the linux userspace GPIO number
     */
    static int rockchipGPIOToNumber(QString gpioName);

private:

    /**
     * @brief addGPIO - add(export) the gpio into the system
     */
    void addGPIO(int gpio);

    /**
     * @brief removeGPIO - remove(unexport) the gpio from the system
     */
    void removeGPIO(int gpio);

    /**
     * @brief setDirection - set the direction of the gpio
     */
    void setDirection(const QString &gpioPath, INSP_GPIO::eGPIO_DIRECTION direction);

    /**
     * @brief setEdge - set the edoe of the gpio
     */
    void setEdge(const QString &gpioPath, INSP_GPIO::eGPIO_EDGE edge);

    /**
     * @brief setActiveLow - set the active low prop of the gpio
     */
    void setActiveLow(const QString &gpioPath, bool activeLow);

    /**
     * @brief setValue - set the value of the gpio
     */
    void setValue(const QString &gpioPath, int value);

    /**
     * @brief getValue - read the value of the gpio
     */
    int getValue(const QString &gpioPath);

private:
    QString mGpioPath;

};

#endif // INSPGPIO_HPP
