#include "button.h"
#include "stm32f4xx.h"

#define BUTTON_PORT GPIOC
#define BUTTON_PIN 13

void button_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    BUTTON_PORT->MODER &= ~(0x3 << (BUTTON_PIN * 2));

    BUTTON_PORT->PUPDR &= ~(0x3 << (BUTTON_PIN * 2));
    BUTTON_PORT->PUPDR |= (0x1 << (BUTTON_PIN * 2));
}

bool button_is_pressed(void)
{
    return (BUTTON_PORT->IDR & (1 << BUTTON_PIN)) == 0;
}