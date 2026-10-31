#include <zephyr/shell/shell.h>
#include "iomico_myled.h"

static int cmd_channel_get_handler(const struct shell* sh, int argc, char** argv)
{
    const struct device* dev = shell_device_get_binding(argv[1]);
    if (!dev) {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    int ret = iomico_myled_state_off(dev);
    
    if (ret != 0) {
        shell_error(sh, "Could not iomico_myled_state_off, ret:%d", ret);
        return -EFAULT;
    }

    return 0;
}

static int cmd_sample_fetch_handler(const struct shell* sh, int argc, char** argv)
{
    const struct device* dev = shell_device_get_binding(argv[1]);
    if (!dev) {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    int ret = iomico_myled_state_on(dev);
    
    if (ret != 0) {
        shell_error(sh, "Could not iomico_myled_state_on, ret:%d", ret);
        return -EFAULT;
    }

    return 0;
}

static int cmd_info_handler(const struct shell* sh, int argc, char** argv)
{
    const struct device* dev = shell_device_get_binding(argv[1]);
    if (!dev) {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    struct iomico_myled_data *myled_data = dev->data;

    shell_info(sh, "Device name: %s", dev->name);
    shell_info(sh, "Ready state: %d", myled_data->gpio_ok);

    return 0;
}

static int cmd_set_handler(const struct shell* sh, int argc, char** argv)
{
    const struct device* dev = shell_device_get_binding(argv[1]);
    if (!dev) {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    int ret;
    long arg = shell_strtol(argv[2], 10, &ret);
	if (ret < 0) {
        shell_error(sh, "Invalid argument %s", argv[2]);
		return -EINVAL;
	}
    
	if (!((arg == 0) || (arg == 1))) {
        shell_error(sh, "Out of range %s", argv[2]);
		return -ERANGE;
    }

    ret = iomico_myled_set_gpio_ok(dev, (bool)arg);
    if (ret != 0) {
        shell_error(sh, "Could not iomico_myled_set_gpio_ok, ret:%d", ret);
        return -EFAULT;
    }

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(iomico_myled_subcmd,
    SHELL_CMD_ARG(read, NULL, "Get channel (turn LED OFF)", cmd_channel_get_handler, 2, 0),
    SHELL_CMD_ARG(fetch, NULL, "Fetch sample (turn LED ON)", cmd_sample_fetch_handler, 2, 0),
    SHELL_CMD_ARG(info, NULL, "Device info", cmd_info_handler, 2, 0),
    SHELL_CMD_ARG(set, NULL, "Set ready state", cmd_set_handler, 3, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(iomico_myled, &iomico_myled_subcmd, "Driver commands", NULL);
