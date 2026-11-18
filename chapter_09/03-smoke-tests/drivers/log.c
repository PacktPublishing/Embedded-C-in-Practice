#include "log.h"
#include "uart.h"
#include "system_timer.h"
#include <stdio.h>

#define LOG_UART           UART_2
#define LOG_UART_BAUDRATE  115200
#define LOG_LINE_SIZE      128

void log_init(void)
{
    static const uart_config_t log_config =
    {
        .baudrate = LOG_UART_BAUDRATE,
        .parity = UART_PARITY_NONE
    };

    uart_init(LOG_UART, &log_config);
}

static const char *level_prefix(log_level_t level)
{
    switch (level)
    {
        case LOG_DEBUG:
            return "[DEBUG] ";

        case LOG_INFO:
            return "[INFO] ";

        case LOG_WARNING:
            return "[WARNING] ";

        case LOG_ERROR:
            return "[ERROR] ";

        default:
            return "";
    }
}

void log_print(log_level_t level, const char *message)
{
    char line[LOG_LINE_SIZE];
    int used = snprintf(line, sizeof(line), "[%08lu] %s%s\r\n",
                        (unsigned long)system_timer_get_ms(),
                        level_prefix(level), message);

    if (used < 0)
    {
        return;
    }

    if ((size_t)used > (sizeof(line) - 1u))
    {
        used = (int)(sizeof(line) - 1u);
    }

    uart_write(LOG_UART, (const uint8_t *)line, (uint32_t)used);
}
