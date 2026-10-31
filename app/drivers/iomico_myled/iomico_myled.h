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
typedef int (*iomico_myled_state_t)(const struct device *dev);
typedef int (*iomico_myled_gpio_ok_t)(const struct device *dev, bool value);

struct iomico_myled_api {
    iomico_myled_state_t   on;
    iomico_myled_state_t   off;
    iomico_myled_gpio_ok_t set_gpio_ok;
    iomico_myled_gpio_ok_t get_gpio_ok;
};

static inline int iomico_myled_state_on(const struct device *dev)
{
    const struct iomico_myled_api *api = (const struct iomico_myled_api *)dev->api;

    if (api == NULL || api->on == NULL) {
        return -ENOSYS;
    }
    return api->on(dev);
}

static inline int iomico_myled_state_off(const struct device *dev)
{
    const struct iomico_myled_api *api = (const struct iomico_myled_api *)dev->api;

    if (api == NULL || api->off == NULL) {
        return -ENOSYS;
    }
    return api->off(dev);
}

static inline int iomico_myled_set_gpio_ok(const struct device *dev, bool value)
{
    const struct iomico_myled_api *api = (const struct iomico_myled_api *)dev->api;

    if (api == NULL || api->set_gpio_ok == NULL) {
        return -ENOSYS;
    }
    return api->set_gpio_ok(dev, value);
}

#ifdef __cplusplus
}
#endif

#endif /* IOMICO_MYLED_H */