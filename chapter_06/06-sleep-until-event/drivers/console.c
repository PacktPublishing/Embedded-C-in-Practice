#include <stdint.h>

#include "uart.h"
#include "console.h"
#include "event_queue.h"

#define CONSOLE_UART            UART_2
#define CONSOLE_IRQ_PRIORITY    5U

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

    if (uart_read_byte(CONSOLE_UART, &byte) == UART_OK)
    {
        const event_t event = { .type = EVENT_UART_RX, .data = byte };

        event_queue_put(&app_events, event);
    }
}