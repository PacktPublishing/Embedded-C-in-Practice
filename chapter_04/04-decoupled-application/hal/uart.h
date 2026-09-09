#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    UART_1,
    UART_2
} uart_instance_t;

typedef enum
{
    UART_PARITY_NONE,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} uart_parity_t;

typedef struct
{
    uint32_t baudrate;
    uint32_t pclk_hz;
    uart_parity_t parity;
} uart_config_t;

typedef struct
{
    uart_instance_t instance;
    uart_config_t config;
    bool initialized;
} uart_handle_t;

typedef enum
{
    UART_OK = 0,
    UART_ERROR_INVALID_ARG,
    UART_ERROR_NOT_INITIALIZED,
    UART_ERROR_TIMEOUT
} uart_status_t;

uart_status_t uart_init(uart_handle_t *uart, uart_instance_t instance,
                        const uart_config_t *config);

uart_status_t uart_write(uart_handle_t *uart, const uint8_t *data,
                         uint32_t len);

uart_status_t uart_read(uart_handle_t *uart, uint8_t *data, uint32_t len);

uart_status_t uart_deinit(uart_handle_t *uart);

#endif // HAL_UART_H
