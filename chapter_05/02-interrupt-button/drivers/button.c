#include "button.h"
#include "led.h"
#include "gpio.h"
#include "exti.h"

#define BUTTON_PORT         GPIO_C
#define BUTTON_PIN          13
#define BUTTON_ACTIVE_LOW   1
#define BUTTON_IRQ_PRIORITY 5U

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

void button_enable_interrupt(void)
{
    const exti_config_t config =
    {
        .trigger = BUTTON_ACTIVE_LOW ? EXTI_TRIGGER_FALLING
                                     : EXTI_TRIGGER_RISING,
        .priority = BUTTON_IRQ_PRIORITY
    };

    exti_configure(BUTTON_PORT, BUTTON_PIN, &config);
}

void EXTI15_10_IRQHandler(void)
{
    if (exti_is_pending(BUTTON_PIN))
    {
        exti_clear_pending(BUTTON_PIN);
        led_toggle();
    }
}