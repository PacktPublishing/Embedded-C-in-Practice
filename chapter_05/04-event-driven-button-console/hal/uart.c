#include "uart.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include <stdbool.h>
#include <stddef.h>

#define UART_AF 7
#define UART_TIMEOUT_GUARD 1000000
#define APB1_CLOCK_HZ 16000000

typedef struct
{
    USART_TypeDef *peripheral;
    uart_config_t config;
    bool initialized;
} uart_handle_t;

static uart_handle_t uart1_handle;
static uart_handle_t uart2_handle;

static uart_handle_t *get_handle(uart_instance_t instance)
{
    switch (instance)
    {
        case UART_1:
            return &uart1_handle;

        case UART_2:
            return &uart2_handle;

        default:
            return NULL;
    }
}

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

uart_status_t uart_init(uart_instance_t instance,
                        const uart_config_t *config)
{
    if (config == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    USART_TypeDef *peripheral = get_instance(instance);
    uart_handle_t *handle = get_handle(instance);

    if ((peripheral == NULL) || (handle == NULL))
    {
        return UART_ERROR_INVALID_ARG;
    }

    if (handle->initialized)
    {
        return UART_OK;
    }

    handle->peripheral = peripheral;
    handle->config = *config;

    configure_pins(instance);
    uart_enable_clock(instance);

    uint32_t brr = (APB1_CLOCK_HZ + (config->baudrate / 2)) / config->baudrate;
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

    handle->initialized = true;

    return UART_OK;
}

uart_status_t uart_write(uart_instance_t instance,
                         const uint8_t *data,
                         uint32_t len)
{
    if (data == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    uart_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

    USART_TypeDef *peripheral = handle->peripheral;

    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t guard = 0;

        while ((peripheral->SR & USART_SR_TXE) == 0)
        {
            guard++;

            if (guard > UART_TIMEOUT_GUARD)
            {
                return UART_ERROR_TIMEOUT;
            }
        }

        peripheral->DR = data[i];
    }

    return UART_OK;
}

uart_status_t uart_read(uart_instance_t instance,
                        uint8_t *data,
                        uint32_t len)
{
    if (data == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    uart_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

    USART_TypeDef *peripheral = handle->peripheral;

    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t guard = 0;

        while ((peripheral->SR & USART_SR_RXNE) == 0)
        {
            guard++;

            if (guard > UART_TIMEOUT_GUARD)
            {
                return UART_ERROR_TIMEOUT;
            }
        }

        data[i] = (uint8_t)peripheral->DR;
    }

    return UART_OK;
}

uart_status_t uart_deinit(uart_instance_t instance)
{
    uart_handle_t *handle = get_handle(instance);

    if (handle == NULL)
    {
        return UART_ERROR_INVALID_ARG;
    }

    if (handle->initialized)
    {
        handle->peripheral->CR1 = 0;
    }

    handle->initialized = false;

    return UART_OK;
}

static IRQn_Type get_irq(uart_instance_t instance)
{
    switch (instance)
    {
        case UART_1:
            return USART1_IRQn;

        default:
            return USART2_IRQn;
    }
}

uart_status_t uart_enable_rx_interrupt(uart_instance_t instance,
                                       uint8_t priority)
{
    uart_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

    IRQn_Type irq = get_irq(instance);

    handle->peripheral->CR1 |= USART_CR1_RXNEIE;

    NVIC_SetPriority(irq, priority);
    NVIC_EnableIRQ(irq);

    return UART_OK;
}