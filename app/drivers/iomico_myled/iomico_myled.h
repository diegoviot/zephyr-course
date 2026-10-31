#ifndef IOMICO_MYLED_H
#define IOMICO_MYLED_H

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct iomico_myled_config {
    struct gpio_dt_spec led_gpio;
};

struct iomico_myled_data {
    bool led_state;
	bool gpio_ok;
};

/* Custom driver API: function-pointer table */
typedef int (*iomico_myled_dev_t)(const struct device *dev);

struct iomico_myled_api {
    iomico_myled_dev_t on;
    iomico_myled_dev_t off;
};

static inline int iomico_myled_on(const struct device *dev)
{
    const struct iomico_myled_api *api = (const struct iomico_myled_api *)dev->api;

    if (api == NULL || api->on == NULL) {
        return -ENOSYS;
    }
    return api->on(dev);
}

static inline int iomico_myled_off(const struct device *dev)
{
    const struct iomico_myled_api *api = (const struct iomico_myled_api *)dev->api;

    if (api == NULL || api->off == NULL) {
        return -ENOSYS;
    }
    return api->off(dev);
}

#ifdef __cplusplus
}
#endif

#endif /* IOMICO_MYLED_H */