#include <stddef.h>
#include <stdint.h>

#include "uart.h"
#include "console.h"

#define CONSOLE_UART            UART_2
#define CONSOLE_IRQ_PRIORITY    5U

static console_callback_t rx_callback = NULL;

void console_set_rx_callback(console_callback_t callback)
{
    rx_callback = callback;
}

void console_init(void)
{
    static const uart_config_t console_config =
    {
        .baudrate = 115200,
        .parity = UART_PARITY_NONE
    };

    uart_init(CONSOLE_UART, &console_config);
    uart_enable_rx_interrupt(CONSOLE_UART, CONSOLE_IRQ_PRIORITY);
}

void USART2_IRQHandler(void)
{
    uint8_t byte;

    if ((uart_read_byte(CONSOLE_UART, &byte) == UART_OK) &&
        (rx_callback != NULL))
    {
        rx_callback(byte);
    }
}