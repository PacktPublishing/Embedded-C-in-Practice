#include "led.h"
#include "stm32f4xx.h"

#define LED_PORT GPIOA
#define LED_PIN 5
#define LED_ACTIVE_HIGH 1

void led_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    LED_PORT->MODER &= ~(0x3 << (LED_PIN * 2));
    LED_PORT->MODER |= (0x1 << (LED_PIN * 2));

    LED_PORT->OTYPER &= ~(0x1 << LED_PIN);
    LED_PORT->OSPEEDR &= ~(0x3 << (LED_PIN * 2));
}

void led_set(bool status)
{
    bool level = LED_ACTIVE_HIGH ? status : !status;

    if (level)
    {
        LED_PORT->BSRR = (1 << LED_PIN);
    }
    else
    {
        LED_PORT->BSRR = (1 << (LED_PIN + 16));
    }
}

void led_toggle(void)
{
    bool raw_high = (LED_PORT->ODR & (1 << LED_PIN)) != 0;
    bool is_on = LED_ACTIVE_HIGH ? raw_high : !raw_high;

    led_set(!is_on);
}