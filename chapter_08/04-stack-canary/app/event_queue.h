#ifndef APP_EVENT_QUEUE_H
#define APP_EVENT_QUEUE_H

#include <stdbool.h>
#include <stdint.h>

#include "app/events.h"

#define EVENT_QUEUE_SIZE 16u

typedef struct
{
    volatile event_t items[EVENT_QUEUE_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
} event_queue_t;

extern event_queue_t app_events;

bool event_queue_put(event_queue_t *q, event_t item);
bool event_queue_get(event_queue_t *q, event_t *item);

#endif // APP_EVENT_QUEUE_H