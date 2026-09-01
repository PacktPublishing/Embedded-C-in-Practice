#include "led.h"
#include "stm32f4xx.h"

void led_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5;
}

void led_set(bool status)
{
    if (status)
    {
        GPIOA->BSRR = GPIO_BSRR_BS5;
    }
    else
    {
        GPIOA->BSRR = GPIO_BSRR_BR5;
    }
}

void led_toggle(void)
{
    bool is_on = (GPIOA->ODR & GPIO_ODR_OD5) != 0;

    led_set(!is_on);
}