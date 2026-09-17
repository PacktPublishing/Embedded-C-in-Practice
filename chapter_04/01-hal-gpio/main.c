#include "gpio.h"

#define LED_PORT GPIO_A
#define LED_PIN 5

int main(void)
{
    const gpio_config_t config =
    {
        .direction = GPIO_DIRECTION_OUTPUT,
        .output_type = GPIO_PUSH_PULL
    };

    gpio_configure(LED_PORT, LED_PIN, &config);

    gpio_write_pin(LED_PORT, LED_PIN, true);
}