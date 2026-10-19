#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>


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

SHELL_STATIC_SUBCMD_SET_CREATE(obled_driver_subcmd,
    /* Add your shell commands here */
    SHELL_CMD_ARG(read, NULL, "Invokes sensor_channel_get: Turn LED OFF", cmd_read, 2, 0),
    SHELL_CMD_ARG(fetch, NULL, "Invokes sensor_sample_fetch: Turn LED ON", cmd_fetch, 2, 0),
    SHELL_CMD_ARG(info, NULL, "Prints device name and ready state", cmd_info, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &obled_driver_subcmd, "obled (on-board LED) control driver commands", NULL);
