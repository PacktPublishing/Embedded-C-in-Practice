#include "event_queue.h"

event_queue_t app_events;

bool event_queue_put(event_queue_t *q, event_t item)
{
    uint32_t next = (q->head + 1u) % EVENT_QUEUE_SIZE;

    if (next == q->tail)
    {
        return false;
    }

    q->items[q->head] = item;
    q->head = next;

    return true;
}

bool event_queue_get(event_queue_t *q, event_t *item)
{
    if (q->tail == q->head)
    {
        return false;
    }

    *item = q->items[q->tail];
    q->tail = (q->tail + 1u) % EVENT_QUEUE_SIZE;

    return true;
}