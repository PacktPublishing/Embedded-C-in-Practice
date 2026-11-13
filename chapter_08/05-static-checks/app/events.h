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
    uint32_t data;
} event_t;

_Static_assert(sizeof(event_t) == 8u,
               "event_t size changed: recheck the queue budget");

#endif // APP_EVENTS_H