#include "button.h"
#include "stm32f4xx.h"

void button_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER &= ~GPIO_MODER_MODER13;

    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0;
}

bool button_is_pressed(void)
{
    return (GPIOC->IDR & GPIO_IDR_ID13) == 0;
}