#include "uart.h"

int main(void)
{
    static const uart_config_t config =
    {
        .baudrate = 115200,
        .parity = UART_PARITY_NONE
    };

    uart_init(UART_2, &config);

    const uint8_t message[] = "UART ready\r\n";

    while (1)
    {
        uart_write(UART_2, message, sizeof(message) - 1);
    }
}