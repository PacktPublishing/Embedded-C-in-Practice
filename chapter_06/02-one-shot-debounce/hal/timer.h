#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    TIMER_2,
    TIMER_3,
    TIMER_4
} timer_instance_t;

typedef enum
{
    TIMER_MODE_PERIODIC,
    TIMER_MODE_ONE_SHOT
} timer_mode_t;

typedef enum
{
    TIMER_OK = 0,
    TIMER_ERROR_INVALID_ARG,
    TIMER_ERROR_NOT_INITIALIZED
} timer_status_t;

typedef struct
{
    timer_mode_t mode;
    uint32_t tick_hz;
    uint32_t period_ticks;
} timer_config_t;

timer_status_t timer_init(timer_instance_t instance,
                          const timer_config_t *config);
void timer_start(timer_instance_t instance);

void timer_enable_update_interrupt(timer_instance_t instance,
                                   uint8_t priority);
bool timer_is_update_pending(timer_instance_t instance);
void timer_clear_update(timer_instance_t instance);

#endif
