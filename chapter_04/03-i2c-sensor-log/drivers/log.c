#include "log.h"
#include "uart.h"
#include <string.h>

#define LOG_UART           UART_2
#define LOG_UART_BAUDRATE  115200

void log_init(void)
{
    static const uart_config_t log_config =
    {
        .baudrate = LOG_UART_BAUDRATE,
        .parity = UART_PARITY_NONE
    };

    uart_init(LOG_UART, &log_config);
}

void log_print(log_level_t level, const char *message)
{
    const char *prefix = "";

    switch (level)
    {
        case LOG_INFO:
            prefix = "[INFO] ";
            break;

        case LOG_WARNING:
            prefix = "[WARNING] ";
            break;

        case LOG_ERROR:
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

    uart_write(LOG_UART, (const uint8_t *)line,
               (uint32_t)(prefix_len + message_len + 2));
}