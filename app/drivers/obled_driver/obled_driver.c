#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "obled_driver.h"

#define DT_DRV_COMPAT obled_driver

LOG_MODULE_REGISTER(obled_driver, LOG_LEVEL_INF);

struct obled_data {
	uint32_t counter;
};

struct obled_config {
	struct gpio_dt_spec led;
};

static int obled_dev_init(const struct device *dev)
{
	const struct obled_config *cfg = dev->config;

	LOG_INF("Initializing on board LED device %s...", dev->name);
	if (!gpio_is_ready_dt(&cfg->led)) {
		return -ENODEV;
	}
	return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

static int obled_driver_channel_get(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val)
{
	const struct obled_config *cfg = dev->config;

	LOG_INF("%s: Turning LED OFF for channel %d", dev->name, chan);

	return gpio_pin_set_dt(&cfg->led, 0);
}

static int obled_driver_sample_fetch(const struct device *dev,
				     enum sensor_channel chan)
{
	const struct obled_config *cfg = dev->config;

	LOG_INF("%s: Turning LED ON for channel %d", dev->name, chan);

	return gpio_pin_set_dt(&cfg->led, 1);
}

static int obled_driver_inc_counter(const struct device *dev, uint32_t *counter)
{
	struct obled_data *data = dev->data;

	data->counter++;
	if (counter != NULL) {
		*counter = data->counter;
	}
	return 0;
}

static int obled_driver_set_counter(const struct device *dev, uint32_t counter)
{
	struct obled_data *data = dev->data;

	data->counter = counter;
	return 0;
}

static const struct obled_driver_api obled_api = {
	.sensor = {
		.channel_get = obled_driver_channel_get,
		.sample_fetch = obled_driver_sample_fetch,
	},
	.inc_counter = obled_driver_inc_counter,
	.set_counter = obled_driver_set_counter,
};

#define OBLED_INST(inst)							\
	static struct obled_data obled_data_##inst;				\
	static const struct obled_config obled_config_##inst = {		\
		.led = GPIO_DT_SPEC_INST_GET(inst, led_gpios),			\
	};									\
	DEVICE_DT_INST_DEFINE(inst, obled_dev_init, NULL,			\
			      &obled_data_##inst, &obled_config_##inst,		\
			      POST_KERNEL, 80, &obled_api);

DT_INST_FOREACH_STATUS_OKAY(OBLED_INST)
