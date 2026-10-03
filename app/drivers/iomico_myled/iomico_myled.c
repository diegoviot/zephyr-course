#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h> 

LOG_MODULE_DECLARE(iomico_myled, LOG_LEVEL_INF);

static int channel_get_iomico_myled(const struct device *dev,
				                    enum sensor_channel chan,
				                    struct sensor_value *val){
    LOG_INF("Iomico - MyLed - channel_get chan:%d", chan);
    return 0;
};

static DEVICE_API(sensor, api_iomico_assignment) = {
    .channel_get = channel_get_iomico_myled,
};

// init function
static int init(const struct device* dev){
    LOG_INF("Device Initialized");
    return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL);
