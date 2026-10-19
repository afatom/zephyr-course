#ifndef OBLED_DRIVER_H_
#define OBLED_DRIVER_H_

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

struct obled_driver_api {
	/* Must stay first: sensor_*() calls cast dev->api to sensor_driver_api. */
	struct sensor_driver_api sensor;
	int (*inc_counter)(const struct device *dev, uint32_t *counter);
	int (*set_counter)(const struct device *dev, uint32_t counter);
};

/** Increment this instance's counter; optionally returns the new value in @p counter. */
static inline int obled_inc_counter(const struct device *dev, uint32_t *counter)
{
	const struct obled_driver_api *api = (const struct obled_driver_api *)dev->api;

	return api->inc_counter(dev, counter);
}

static inline int obled_set_counter(const struct device *dev, uint32_t counter)
{
	const struct obled_driver_api *api = (const struct obled_driver_api *)dev->api;

	return api->set_counter(dev, counter);
}
#ifdef __cplusplus
}
#endif

#endif /* OBLED_DRIVER_H_ */
