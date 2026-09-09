#include "uart.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include <stddef.h>

#define UART_AF 7

static USART_TypeDef *get_instance(uart_instance_t instance)
{
    switch (instance)
    {
        case UART_1:
            return USART1;

        case UART_2:
            return USART2;

        default:
            return NULL;
    }
}

static void configure_pins(uart_instance_t instance)
{
    switch (instance)
    {
        case UART_1:
            gpio_configure_alternate(GPIO_A, 9, UART_AF, GPIO_PUSH_PULL);
            gpio_configure_alternate(GPIO_A, 10, UART_AF, GPIO_PUSH_PULL);
            break;

        case UART_2:
            gpio_configure_alternate(GPIO_A, 2, UART_AF, GPIO_PUSH_PULL);
            gpio_configure_alternate(GPIO_A, 3, UART_AF, GPIO_PUSH_PULL);
            break;

        default:
            break;
    }
}

static void uart_enable_clock(uart_instance_t instance)
{
    switch (instance)
    {
        case UART_1:
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            break;

        case UART_2:
            RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
            break;

        default:
            break;
    }
}

uart_status_t uart_init(uart_handle_t *uart, uart_instance_t instance,
                        const uart_config_t *config)
{
    if ((uart == NULL) || (config == NULL))
    {
        return UART_ERROR_INVALID_ARG;
    }

    USART_TypeDef *peripheral = get_instance(instance);

    if (peripheral == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    uart->instance = instance;
    uart->config = *config;

    configure_pins(instance);
    uart_enable_clock(instance);

    uint32_t brr = (config->pclk_hz + (config->baudrate / 2)) / config->baudrate;
    peripheral->BRR = brr;

    uint32_t cr1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    if (config->parity != UART_PARITY_NONE)
    {
        cr1 |= USART_CR1_PCE;

        if (config->parity == UART_PARITY_ODD)
        {
            cr1 |= USART_CR1_PS;
        }
    }

    peripheral->CR1 = cr1;

    uart->initialized = true;

    return UART_OK;
}

uart_status_t uart_write(uart_handle_t *uart, const uint8_t *data,
                         uint32_t len)
{
    if ((uart == NULL) || (data == NULL))
    {
        return UART_ERROR_INVALID_ARG;
    }

    if (!uart->initialized)
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

    USART_TypeDef *peripheral = get_instance(uart->instance);

    if (peripheral == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t guard = 0;

        while ((peripheral->SR & USART_SR_TXE) == 0)
        {
            guard++;

            if (guard > 1000000)
            {
                return UART_ERROR_TIMEOUT;
            }
        }

        peripheral->DR = data[i];
    }

    return UART_OK;
}

uart_status_t uart_read(uart_handle_t *uart, uint8_t *data, uint32_t len)
{
    if ((uart == NULL) || (data == NULL))
    {
        return UART_ERROR_INVALID_ARG;
    }

    if (!uart->initialized)
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

    USART_TypeDef *peripheral = get_instance(uart->instance);

    if (peripheral == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t guard = 0;

        while ((peripheral->SR & USART_SR_RXNE) == 0)
        {
            guard++;

            if (guard > 1000000)
            {
                return UART_ERROR_TIMEOUT;
            }
        }

        data[i] = (uint8_t)peripheral->DR;
    }

    return UART_OK;
}

uart_status_t uart_deinit(uart_handle_t *uart)
{
    if (uart == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    USART_TypeDef *peripheral = get_instance(uart->instance);

    if (peripheral != NULL)
    {
        peripheral->CR1 = 0;
    }

    uart->initialized = false;

    return UART_OK;
}
