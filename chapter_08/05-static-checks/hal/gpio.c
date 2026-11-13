#include "gpio.h"

#include <stddef.h>

#include "stm32f4xx.h"

static GPIO_TypeDef *get_instance(gpio_port_t port)
{
    switch (port)
    {
        case GPIO_A:
            return GPIOA;

        case GPIO_B:
            return GPIOB;

        case GPIO_C:
            return GPIOC;

        case GPIO_D:
            return GPIOD;

        default:
            return NULL;
    }
}

static void gpio_enable_clock(GPIO_TypeDef *instance)
{
    if (instance == GPIOA)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    }
    else if (instance == GPIOB)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    }
    else if (instance == GPIOC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    }
    else if (instance == GPIOD)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }

    /* Read back so the clock is running before the first register access. */
    (void)RCC->AHB1ENR;
}

void gpio_configure(gpio_port_t port, uint8_t pin,
                    const gpio_config_t *config)
{
    GPIO_TypeDef *instance = get_instance(port);

    if ((instance == NULL) || (config == NULL))
    {
        return;
    }

    gpio_enable_clock(instance);

    instance->MODER &= ~(0x3u << (pin * 2u));

    if (config->direction == GPIO_DIRECTION_OUTPUT)
    {
        instance->MODER |= (0x1u << (pin * 2u));

        instance->OTYPER &= ~(0x1u << pin);
        instance->OTYPER |= ((uint32_t)config->output_type << pin);

        instance->OSPEEDR &= ~(0x3u << (pin * 2u));
    }
    else
    {
        instance->PUPDR &= ~(0x3u << (pin * 2u));
        instance->PUPDR |= ((uint32_t)config->pull << (pin * 2u));
    }
}

void gpio_configure_alternate(gpio_port_t port, uint8_t pin,
                              uint8_t function, gpio_output_type_t type)
{
    GPIO_TypeDef *instance = get_instance(port);

    if (instance == NULL)
    {
        return;
    }

    gpio_enable_clock(instance);

    instance->MODER &= ~(0x3u << (pin * 2u));
    instance->MODER |= (0x2u << (pin * 2u));

    instance->OTYPER &= ~(0x1u << pin);
    instance->OTYPER |= ((uint32_t)type << pin);

    uint32_t reg = pin / 8u;
    uint32_t shift = (pin % 8u) * 4u;

    instance->AFR[reg] &= ~(0xFu << shift);
    instance->AFR[reg] |= ((uint32_t)function << shift);
}

void gpio_configure_analog(gpio_port_t port, uint8_t pin)
{
    GPIO_TypeDef *instance = get_instance(port);

    if (instance == NULL)
    {
        return;
    }

    gpio_enable_clock(instance);

    instance->PUPDR &= ~(0x3u << (pin * 2u));
    instance->MODER |= (0x3u << (pin * 2u));
}

void gpio_write_pin(gpio_port_t port, uint8_t pin, bool level)
{
    GPIO_TypeDef *instance = get_instance(port);

    if (instance == NULL)
    {
        return;
    }

    if (level)
    {
        instance->BSRR = (1u << pin);
    }
    else
    {
        instance->BSRR = (1u << (pin + 16u));
    }
}

bool gpio_read_pin(gpio_port_t port, uint8_t pin)
{
    GPIO_TypeDef *instance = get_instance(port);

    if (instance == NULL)
    {
        return false;
    }

    return (instance->IDR & (1u << pin)) != 0u;
}
