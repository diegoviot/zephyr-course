#ifndef IOMICO_MYLED_H
#define IOMICO_MYLED_H

#include <zephyr/drivers/gpio.h>

struct iomico_myled_config {
    struct gpio_dt_spec led_gpio;
};

struct iomico_myled_data {
    bool led_state;
	bool gpio_ok;
};

#endif /* IOMICO_MYLED_H */