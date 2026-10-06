#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    void test() {
        const struct device* driver0 = DEVICE_DT_GET(DT_NODELABEL(iomico_myled0));
        const struct device* driver1 = DEVICE_DT_GET(DT_NODELABEL(iomico_myled1));
        struct sensor_value val;
        
        LOG_INF("Calling sensor_sample_fetch (turn LEDs ON)...");
        auto ret1 = sensor_sample_fetch(driver0);
        auto ret2 = sensor_sample_fetch(driver1);
        LOG_INF("sensor_sample_fetch Return: %d %d", ret1, ret2);
        
        k_msleep(500);
        
        LOG_INF("Calling sensor_channel_get (turn LEDs OFF)...");
        ret1 = sensor_channel_get(driver0, SENSOR_CHAN_AMBIENT_TEMP, &val);
        ret2 = sensor_channel_get(driver1, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("sensor_channel_get Return: %d %d", ret1, ret2);
    }
}

int main(void)
{
    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    test();

    while (1) {
        if (gpio_pin_set_dt(&led, 1) < 0) return 0;
        k_msleep(25);
        if (gpio_pin_set_dt(&led, 0) < 0) return 0;
        k_msleep(25);
        if (gpio_pin_set_dt(&led, 1) < 0) return 0;
        k_msleep(25);
        if (gpio_pin_set_dt(&led, 0) < 0) return 0;
        k_msleep(25);

        LOG_INF("LED heartbeat!");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS - 100);
    }
    return 0;
}
