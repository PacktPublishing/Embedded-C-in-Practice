#include <stddef.h>

#include "button.h"
#include "gpio.h"
#include "exti.h"
#include "timer.h"

#define BUTTON_PORT         GPIO_C
#define BUTTON_PIN          13
#define BUTTON_ACTIVE_LOW   1
#define BUTTON_IRQ_PRIORITY 5U

#define BUTTON_DEBOUNCE_TIMER TIMER_4
#define BUTTON_DEBOUNCE_MS 20u

static button_callback_t press_callback = NULL;

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

static void configure_button_exti(void)
{
    const exti_config_t config =
    {
        .trigger = BUTTON_ACTIVE_LOW ? EXTI_TRIGGER_FALLING
                                     : EXTI_TRIGGER_RISING,
        .priority = BUTTON_IRQ_PRIORITY
    };

    exti_configure(BUTTON_PORT, BUTTON_PIN, &config);
}

/*
 * Register the callback before button_enable_interrupt(): the handler
 * reads the pointer, and it must be in place before the first press.
 */
void button_set_press_callback(button_callback_t callback)
{
    press_callback = callback;
}

static void on_debounce_expired(void)
{
    if (button_is_pressed() && (press_callback != NULL))
    {
        press_callback();
    }
}

void button_enable_interrupt(void)
{
    const timer_config_t debounce =
    {
        .mode = TIMER_MODE_ONE_SHOT,
        .tick_hz = 1000u,
        .period_ticks = BUTTON_DEBOUNCE_MS
    };

    timer_init(BUTTON_DEBOUNCE_TIMER, &debounce);
    timer_set_update_callback(BUTTON_DEBOUNCE_TIMER, on_debounce_expired);
    timer_enable_update_interrupt(BUTTON_DEBOUNCE_TIMER,
                                  BUTTON_IRQ_PRIORITY);

    configure_button_exti();
}

void EXTI15_10_IRQHandler(void)
{
    if (exti_is_pending(BUTTON_PIN))
    {
        exti_clear_pending(BUTTON_PIN);
        timer_start(BUTTON_DEBOUNCE_TIMER);
    }
}
