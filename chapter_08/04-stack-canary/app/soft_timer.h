#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t interval_ms;
    uint32_t last_run;
} soft_timer_t;

void soft_timer_start(soft_timer_t *timer, uint32_t now);
bool soft_timer_expired(soft_timer_t *timer, uint32_t now);

#endif
