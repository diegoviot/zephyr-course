#ifndef IOMICO_MYLED_H
#define IOMICO_MYLED_H

#include <stddef.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

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

typedef int (*iomico_myled_gpio_ok_t)(const struct device *dev, bool value);

struct iomico_myled_api {
    iomico_myled_gpio_ok_t set_gpio_ok;
};

struct iomico_myled_custom_api {
    struct sensor_driver_api sensor;
    struct iomico_myled_api  ext;
};

BUILD_ASSERT(offsetof(struct iomico_myled_custom_api, sensor) == 0,
             "sensor_driver_api must be at offset 0 for sensor subsystem casts");

static inline const struct iomico_myled_api *iomico_myled_ext(const struct device *dev)
{
    const struct iomico_myled_custom_api *full =
        (const struct iomico_myled_custom_api *)dev->api;

    return (full == NULL) ? NULL : &full->ext;
}

static inline int iomico_myled_state_on(const struct device *dev)
{
    return sensor_sample_fetch(dev);
}

static inline int iomico_myled_state_off(const struct device *dev)
{
    struct sensor_value val;

    return sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
}

static inline int iomico_myled_set_gpio_ok(const struct device *dev, bool value)
{
    const struct iomico_myled_api *api = iomico_myled_ext(dev);

    if (api == NULL || api->set_gpio_ok == NULL) {
        return -ENOSYS;
    }
    return api->set_gpio_ok(dev, value);
}

#ifdef __cplusplus
}
#endif

#endif /* IOMICO_MYLED_H */
