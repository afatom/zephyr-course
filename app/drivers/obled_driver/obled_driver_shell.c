#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <errno.h>
#include <stdlib.h>
#include "obled_driver.h"

#define OBLED_COUNTER_MAX 1000000UL


static int cmd_read(const struct shell *shell, size_t argc, char **argv)
{
    /* argv[1] should be the device name (from the device tree) */
    const struct device *dev = device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device (%s) not found", argv[1]);
        return -ENODEV;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

    if (ret) {
        shell_error(shell, "Failed to get sensor channel (ret=%d)", ret);
        return -EFAULT;
    }

    shell_info(shell, "Sensor value: %d", val.val1);
    return 0;
}


static int cmd_fetch(const struct shell *shell, size_t argc, char **argv)
{
    /* argv[1] should be the device name (from the device tree) */
    const struct device *dev = device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device (%s) not found", argv[1]);
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret) {
        shell_error(shell, "Failed to fetch sensor sample (ret=%d)", ret);
        return -EFAULT;
    }

    shell_info(shell, "Sensor sample fetched successfully");
    return 0;
}

static int cmd_info(const struct shell *shell, size_t argc, char **argv)
{
    /* argv[1] should be the device name (from the device tree) */
    const struct device *dev = device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device (%s) not found", argv[1]);
        return -ENODEV;
    }

    shell_info(shell, "Device name: %s", dev->name);
    shell_info(shell, "Device ready: %s", device_is_ready(dev) ? "yes" : "no");
    return 0;
}

static int cmd_set(const struct shell *shell, size_t argc, char **argv)
{
    /* argv[1] should be the device name (from the device tree) */
    const struct device *dev = device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device (%s) not found", argv[1]);
        return -ENODEV;
    }

    char *end;
    const char *arg = argv[2];

    if (arg[0] == '\0') {
        shell_error(shell, "Counter value is missing");
        return -EINVAL;
    }

    /* strtoul silently accepts a leading '-', so reject it explicitly. */
    if (arg[0] == '-') {
        shell_error(shell, "Counter must not be negative: %s", arg);
        return -EINVAL;
    }

    errno = 0;
    unsigned long parsed = strtoul(arg, &end, 10);

    if (end == arg || *end != '\0') {
        shell_error(shell, "Invalid number: '%s'", arg);
        return -EINVAL;
    }

    if (errno == ERANGE || parsed > OBLED_COUNTER_MAX) {
        shell_error(shell, "Counter out of range (0..%lu): %s", OBLED_COUNTER_MAX, arg);
        return -ERANGE;
    }

    uint32_t counter = (uint32_t)parsed;
    int ret = obled_set_counter(dev, counter);
    if (ret) {
        shell_error(shell, "Failed to set LED counter (ret=%d)", ret);
        return -EFAULT;
    }

    shell_info(shell, "LED counter set to %u", counter);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(obled_driver_subcmd,
    /* Add your shell commands here */
    SHELL_CMD_ARG(read, NULL, "Invokes sensor_channel_get: Turn LED OFF", cmd_read, 2, 0),
    SHELL_CMD_ARG(fetch, NULL, "Invokes sensor_sample_fetch: Turn LED ON", cmd_fetch, 2, 0),
    SHELL_CMD_ARG(set, NULL, "Sets the LED counter\nUsage: set <device> <0..1000000>", cmd_set, 3, 0),
    SHELL_CMD_ARG(info, NULL, "Prints device name and ready state", cmd_info, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &obled_driver_subcmd, "obled (on-board LED) control driver commands", NULL);
