#include "exti.h"

#include "stm32f4xx.h"

static uint32_t get_port_code(gpio_port_t port)
{
    switch (port)
    {
        case GPIO_A:
            return 0U;

        case GPIO_B:
            return 1U;

        case GPIO_C:
            return 2U;

        case GPIO_D:
            return 3U;

        default:
            return 0U;
    }
}

static IRQn_Type get_irq(uint8_t pin)
{
    if (pin >= 10U)
    {
        return EXTI15_10_IRQn;
    }
    else if (pin >= 5U)
    {
        return EXTI9_5_IRQn;
    }
    else
    {
        return (IRQn_Type)(EXTI0_IRQn + pin);
    }
}

void exti_configure(gpio_port_t port, uint8_t pin,
                    const exti_config_t *config)
{
    uint32_t reg = pin / 4U;
    uint32_t shift = (pin % 4U) * 4U;
    uint32_t mask = 1U << pin;

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    (void)RCC->APB2ENR;

    SYSCFG->EXTICR[reg] &= ~(0xFU << shift);
    SYSCFG->EXTICR[reg] |= (get_port_code(port) << shift);

    EXTI->RTSR &= ~mask;
    EXTI->FTSR &= ~mask;

    if (config->trigger != EXTI_TRIGGER_FALLING)
    {
        EXTI->RTSR |= mask;
    }

    if (config->trigger != EXTI_TRIGGER_RISING)
    {
        EXTI->FTSR |= mask;
    }

    EXTI->PR = mask;
    EXTI->IMR |= mask;

    IRQn_Type irq = get_irq(pin);

    NVIC_SetPriority(irq, config->priority);
    NVIC_EnableIRQ(irq);
}

bool exti_is_pending(uint8_t pin)
{
    return (EXTI->PR & (1U << pin)) != 0U;
}

void exti_clear_pending(uint8_t pin)
{
    EXTI->PR = (1U << pin);
}