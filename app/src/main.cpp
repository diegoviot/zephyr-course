#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "iomico_myled.h"

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    void test() {
        const struct device* driver0 = DEVICE_DT_GET(DT_NODELABEL(iomico_myled0));
        const struct device* driver1 = DEVICE_DT_GET(DT_NODELABEL(iomico_myled1));
        
        LOG_INF("Calling iomico_myled_on (turn LEDs ON)...");
        auto ret1 = iomico_myled_on(driver0);
        auto ret2 = iomico_myled_on(driver1);
        LOG_INF("iomico_myled_on Return: %d %d", ret1, ret2);
        
        k_msleep(500);
        
        LOG_INF("Calling iomico_myled_off (turn LEDs OFF)...");
        ret1 = iomico_myled_off(driver0);
        ret2 = iomico_myled_off(driver1);
        LOG_INF("iomico_myled_off Return: %d %d", ret1, ret2);
    }
}

int main(void)
{
    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    test();

    int32_t beat_ms = CONFIG_APP_HEARTBEAT_PERIOD_MS * 15 / 100;
    int32_t sleep_ms = CONFIG_APP_HEARTBEAT_PERIOD_MS - (beat_ms * 4);

    LOG_INF("beat_ms:%d sleep_ms:%d\n", beat_ms, sleep_ms);

    uint8_t cnt = 0;
    bool heartbeat = true;
    while (1) {
        if (heartbeat) {
            if (gpio_pin_set_dt(&led, 1) < 0) return 0;
            k_msleep(beat_ms);
            if (gpio_pin_set_dt(&led, 0) < 0) return 0;
            k_msleep(beat_ms);
            if (gpio_pin_set_dt(&led, 1) < 0) return 0;
            k_msleep(beat_ms);
            if (gpio_pin_set_dt(&led, 0) < 0) return 0;
            k_msleep(beat_ms);

            LOG_INF("LED heartbeat #%d!", ++cnt);
            if (cnt >= 10) heartbeat = false;
        }
        
        k_msleep(sleep_ms);
    }
    return 0;
}
