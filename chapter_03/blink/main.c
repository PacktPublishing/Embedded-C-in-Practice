#include <stdint.h>

#include "stm32f401xe.h"

#define LED_PIN 5U

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 500000U; ++i)
    {
        __asm volatile ("nop");
    }
}

int main(void)
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

    while (1)
    {
        /* LED ON. */
        GPIOA->BSRR = (1U << LED_PIN);

        delay();

        /* LED OFF. */
        GPIOA->BSRR = (1U << (LED_PIN + 16U));

        delay();
    }
}
