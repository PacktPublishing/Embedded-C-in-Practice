#include "led.h"
#include "gpio.h"

#define LED_PORT GPIO_A
#define LED_PIN 5
#define LED_ACTIVE_HIGH 1

void led_init(void)
{
    const gpio_config_t config = { .direction = GPIO_DIRECTION_OUTPUT,
                                   .output_type = GPIO_PUSH_PULL };
    gpio_configure(LED_PORT, LED_PIN, &config);
}

void led_set(bool status)
{
    bool level = LED_ACTIVE_HIGH ? status : !status;

    gpio_write_pin(LED_PORT, LED_PIN, level);
}

void led_toggle(void)
{
    bool current_level = gpio_read_pin(LED_PORT, LED_PIN);
    bool current_status = LED_ACTIVE_HIGH ? current_level : !current_level;

    led_set(!current_status);
}