#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#include <stdint.h>

typedef enum
{
    EVENT_BUTTON_PRESSED,
    EVENT_UART_RX,
    EVENT_SOIL_SAMPLE_READY
} event_type_t;

typedef struct
{
    event_type_t type;
    uint8_t data;
} event_t;

#endif // APP_EVENTS_H