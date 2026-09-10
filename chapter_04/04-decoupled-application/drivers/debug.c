#include "debug.h"
#include "uart.h"
#include <string.h>

#define DEBUG_UART           UART_2
#define DEBUG_UART_PCLK_HZ   16000000
#define DEBUG_UART_BAUDRATE  115200

void debug_init(void)
{
    static const uart_config_t debug_config =
    {
        .baudrate = DEBUG_UART_BAUDRATE,
        .parity = UART_PARITY_NONE
    };

    uart_init(DEBUG_UART, &debug_config);
}

void debug_print(debug_level_t level, const char *message)
{
    const char *prefix = "";

    switch (level)
    {
        case DEBUG_INFO:
            prefix = "[INFO] ";
            break;

        case DEBUG_WARNING:
            prefix = "[WARNING] ";
            break;

        case DEBUG_ERROR:
            prefix = "[ERROR] ";
            break;

        default:
            break;
    }

    char line[128];
    size_t prefix_len = strlen(prefix);
    size_t message_len = strlen(message);

    if ((prefix_len + message_len + 2) > sizeof(line))
    {
        message_len = sizeof(line) - prefix_len - 2;
    }

    memcpy(line, prefix, prefix_len);
    memcpy(line + prefix_len, message, message_len);
    line[prefix_len + message_len] = '\r';
    line[prefix_len + message_len + 1] = '\n';

    uart_write(DEBUG_UART, (const uint8_t *)line,
               (uint32_t)(prefix_len + message_len + 2));
}