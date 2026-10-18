// #include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>

/**
 * Home Work: Module 06 - Zephyr Driver Development
 * Controlling on board LED0 via GPIO device driver
 */

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


int main(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(obled_driver0));
    struct sensor_value val;

    LOG_INF("Testing on board LED (OBLED) device driver...");

    if (!device_is_ready(dev)) {
        LOG_ERR("OBLED device not ready");
        return -ENODEV;
    }

    LOG_INF("Starting LED0 test loop...");

    while (1) {
        /* Make sure LED0 is OFF */
        sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
        /* wait for a short period to ensure the LED is off */
        k_msleep(2500);
        /* Turn LED0 ON and wait for a while */
        sensor_sample_fetch(dev);
        k_msleep(250);
    }

    return 0;
}
