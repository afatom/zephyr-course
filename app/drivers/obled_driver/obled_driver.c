#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>


#define DT_DRV_COMPAT obled_driver
#define APP_LED DT_ALIAS(app_led)

LOG_MODULE_REGISTER(obled_driver, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(APP_LED, gpios);

static int obled_dev_init(const struct device *dev) {
	LOG_INF("Initializing on board LED device...");
	if (!gpio_is_ready_dt(&led)) return 0;

    	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

	gpio_pin_set_dt(&led ,0);

	return 0;
}

static int obled_driver_channel_get(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val)
{
	/* Implement channel get: Turn onboard LED0 OFF */
	LOG_INF("Channel get: Turning LED0 OFF for channel %d", chan);

	gpio_pin_set_dt(&led ,0);
	return 0;
}

static int obled_driver_sample_fetch(const struct device *dev,
				     enum sensor_channel chan)
{
	LOG_INF("Sample fetch: Turning LED0 ON for channel %d", chan);

	gpio_pin_set_dt(&led ,1);
	return 0;
}

static DEVICE_API(sensor, obled_driver_api) = {
	.channel_get = obled_driver_channel_get,
	.sample_fetch = obled_driver_sample_fetch,
};

DEVICE_DT_INST_DEFINE(0,
		      obled_dev_init,
		      NULL,
		      NULL,
		      NULL,
		      POST_KERNEL, 80, &obled_driver_api);
