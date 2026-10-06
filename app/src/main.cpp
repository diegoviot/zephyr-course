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
        const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(iomico_myled0));
        struct sensor_value val;
        auto ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channel Return: %d", ret);
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
