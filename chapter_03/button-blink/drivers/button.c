#include "button.h"

#include "stm32f401xe.h"

#define BUTTON_PIN 13U

void button_init(void)
{
    /* Enable GPIOC clock. */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* Configure PC13 as input. */
    GPIOC->MODER &= ~(3U << (BUTTON_PIN * 2U));

    /* Enable pull-up. */
    GPIOC->PUPDR &= ~(3U << (BUTTON_PIN * 2U));
    GPIOC->PUPDR |=  (1U << (BUTTON_PIN * 2U));
}

bool button_is_pressed(void)
{
    /*
     * B1 is active-low:
     * 0 = pressed
     * 1 = released
     */
    return (GPIOC->IDR & (1U << BUTTON_PIN)) == 0U;
}
