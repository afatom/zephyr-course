// #include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>

#include "obled_driver.h"

/**
 * Home Work: Module 06 - Zephyr Driver Development
 * Controlling on board LED0 via GPIO device driver
 */

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


int main(void)
{
    const struct device *devs[] = {
        DEVICE_DT_GET(DT_NODELABEL(obled_driver0)),
        DEVICE_DT_GET(DT_NODELABEL(obled_driver1)),
    };
    struct sensor_value val;

    LOG_INF("Testing on board LED (OBLED) device driver...");

    for (const struct device *dev : devs) {
        if (!device_is_ready(dev)) {
            LOG_ERR("%s not ready", dev->name);
            return -ENODEV;
        }
    }

    while (1) {
        for (const struct device *dev : devs) {
            uint32_t cnt = 0;
            /**
             * Since our new shell commands (sensor read/fetch) manipulate the same
             * onboard LEDs Give the shell commands priority and time so we can test
             * them without coliding with the llogic below Hence Sleep for 60 seconds
             * I could remove the entire logic below and keep the shell clean without
             * all the LOGs below, Since this wasnt comunicated explicitly during the
             * lecture i will keep it AS IS
            */
            k_msleep(1000 * 60);
            sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
            k_msleep(1000);
            sensor_sample_fetch(dev);
            obled_inc_counter(dev, &cnt);
            LOG_INF("%s counter = %u", dev->name, cnt);
            k_msleep(250);
        }
    }

    return 0;
}
