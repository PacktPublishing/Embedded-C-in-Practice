#include "gpio.h"
#include "stm32f4xx.h"
#include <stddef.h>

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
    uint32_t index = ((uint32_t)instance - GPIOA_BASE) / 0x400;

    RCC->AHB1ENR |= (1 << index);
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

    instance->MODER &= ~(0x3 << (pin * 2));

    if (config->direction == GPIO_DIRECTION_OUTPUT)
    {
        instance->MODER |= (0x1 << (pin * 2));

        instance->OTYPER &= ~(0x1 << pin);
        instance->OTYPER |= ((uint32_t)config->output_type << pin);

        instance->OSPEEDR &= ~(0x3 << (pin * 2));
    }
    else
    {
        instance->PUPDR &= ~(0x3 << (pin * 2));
        instance->PUPDR |= ((uint32_t)config->pull << (pin * 2));
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

    instance->MODER &= ~(0x3 << (pin * 2));
    instance->MODER |= (0x2 << (pin * 2));

    instance->OTYPER &= ~(0x1 << pin);
    instance->OTYPER |= ((uint32_t)type << pin);

    uint32_t reg = pin / 8;
    uint32_t shift = (pin % 8) * 4;

    instance->AFR[reg] &= ~(0xF << shift);
    instance->AFR[reg] |= ((uint32_t)function << shift);
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
        instance->BSRR = (1U << pin);
    }
    else
    {
        instance->BSRR = (1U << (pin + 16U));
    }
}

bool gpio_read_pin(gpio_port_t port, uint8_t pin)
{
    GPIO_TypeDef *instance = get_instance(port);

    if (instance == NULL)
    {
        return false;
    }

    return (instance->IDR & (1U << pin)) != 0;
}
