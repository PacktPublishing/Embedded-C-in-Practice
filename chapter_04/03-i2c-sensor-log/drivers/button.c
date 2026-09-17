#include "button.h"
#include "gpio.h"

#define BUTTON_PORT GPIO_C
#define BUTTON_PIN 13
#define BUTTON_ACTIVE_LOW 1

void button_init(void)
{
    const gpio_config_t config = { .direction = GPIO_DIRECTION_INPUT,
                                   .pull = GPIO_PULL_UP };
    gpio_configure(BUTTON_PORT, BUTTON_PIN, &config);
}

bool button_is_pressed(void)
{
    bool level = gpio_read_pin(BUTTON_PORT, BUTTON_PIN);

    return BUTTON_ACTIVE_LOW ? !level : level;
}
