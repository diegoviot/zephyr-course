#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h> 
#include "iomico_myled.h"

#define DT_DRV_COMPAT iomico_myled

LOG_MODULE_REGISTER(iomico_myled, LOG_LEVEL_DBG);

static uint32_t driver_cnt = 0;

static int sample_fetch_iomico_myled(const struct device *dev, enum sensor_channel chan)
{
	struct iomico_myled_data *myled_data = dev->data;

	LOG_INF("Iomico - MyLed - sample_fetch - turning LED ON");
    if(myled_data->gpio_ok) gpio_pin_set_dt(&myled_data->led_gpio, 1);
	myled_data->led_state = true;
	driver_cnt++;
	LOG_DBG("led_state:%d driver_cnt:%u", myled_data->led_state, driver_cnt);
	return 0;
}

static int channel_get_iomico_myled(const struct device *dev,
				                    enum sensor_channel chan,
				                    struct sensor_value *val)
{
	struct iomico_myled_data *myled_data = dev->data;

	LOG_INF("Iomico - MyLed - channel_get - turning LED OFF");
    if(myled_data->gpio_ok) gpio_pin_set_dt(&myled_data->led_gpio, 0);
	myled_data->led_state = false;
	driver_cnt++;
	LOG_DBG("led_state:%d driver_cnt:%u", myled_data->led_state, driver_cnt);
	return 0;
}

static DEVICE_API(sensor, api_iomico_assignment) = {
	.sample_fetch = sample_fetch_iomico_myled,
	.channel_get = channel_get_iomico_myled,
};

// init function
static int init(const struct device* dev)
{
	struct iomico_myled_data *myled_data = dev->data;

	LOG_INF("Device Initialized");
	
	// Check if GPIO is available in devicetree
	if (myled_data->led_gpio.port != NULL && device_is_ready(myled_data->led_gpio.port)) {
		LOG_INF("GPIO device is ready");
		// Configure GPIO as output
		if (gpio_pin_configure_dt(&myled_data->led_gpio, GPIO_OUTPUT_INACTIVE) != 0) {
			LOG_WRN("Failed to configure GPIO");
            myled_data->gpio_ok = false;
        }
        else {
            myled_data->gpio_ok = true;
        }
    }
	return 0;
}

#define DEV_INST(inst) \
	static struct iomico_myled_data iomico_myled_data_##inst = { \
		.led_gpio = GPIO_DT_SPEC_INST_GET_OR(inst, gpios, {0}), \
	}; \
	DEVICE_DT_INST_DEFINE(inst, init, NULL, &iomico_myled_data_##inst, NULL, POST_KERNEL, 80, &api_iomico_assignment);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);
