#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h> 

#define DT_DRV_COMPAT iomico_myled

LOG_MODULE_REGISTER(iomico_myled, LOG_LEVEL_INF);

static const struct gpio_dt_spec led_gpio = GPIO_DT_SPEC_INST_GET_OR(0, gpios, {0});

static int sample_fetch_iomico_myled(const struct device *dev, enum sensor_channel chan)
{	
	LOG_INF("Iomico - MyLed - sample_fetch - turning LED ON");
    gpio_pin_set_dt(&led_gpio, 1);
	return 0;
}

static int channel_get_iomico_myled(const struct device *dev,
				                    enum sensor_channel chan,
				                    struct sensor_value *val)
{	
	LOG_INF("Iomico - MyLed - channel_get - turning LED OFF");
    gpio_pin_set_dt(&led_gpio, 0);
	return 0;
}

static DEVICE_API(sensor, api_iomico_assignment) = {
	.sample_fetch = sample_fetch_iomico_myled,
	.channel_get = channel_get_iomico_myled,
};

// init function
static int init(const struct device* dev)
{	
	LOG_INF("Device Initialized");
	
	// Check if GPIO is available in devicetree
	if (led_gpio.port != NULL && device_is_ready(led_gpio.port)) {
		LOG_INF("GPIO device is ready");
		// Configure GPIO as output
		if (gpio_pin_configure_dt(&led_gpio, GPIO_OUTPUT_INACTIVE) < 0) {
			LOG_WRN("Failed to configure GPIO");
        }
    }
	return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_assignment);