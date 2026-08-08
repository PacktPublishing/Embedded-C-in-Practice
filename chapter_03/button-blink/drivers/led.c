#include "led.h"

#include "stm32f401xe.h"

#define LED_PIN 5U

void led_init(void)
{
    /* Enable GPIOA clock. */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure PA5 as general-purpose output. */
    GPIOA->MODER &= ~(3U << (LED_PIN * 2U));
    GPIOA->MODER |=  (1U << (LED_PIN * 2U));

    /* Push-pull output. */
    GPIOA->OTYPER &= ~(1U << LED_PIN);

    /* No pull-up / pull-down. */
    GPIOA->PUPDR &= ~(3U << (LED_PIN * 2U));

    led_set(false);
}

void led_set(bool state)
{
    if (state)
    {
        GPIOA->BSRR = (1U << LED_PIN);
    }
    else
    {
        GPIOA->BSRR = (1U << (LED_PIN + 16U));
    }
}

void led_toggle(void)
{
    GPIOA->ODR ^= (1U << LED_PIN);
}
