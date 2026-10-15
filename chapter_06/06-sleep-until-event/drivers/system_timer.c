
#include "system_timer.h"

#include "systick.h"

#ifndef SYSTEM_TIMER_START_MS
#define SYSTEM_TIMER_START_MS 0u
#endif

#define SYSTEM_TIMER_TICK_HZ 1000u

static volatile uint32_t ticks_ms = SYSTEM_TIMER_START_MS;

static void system_timer_tick_callback(void)
{
    ticks_ms++;
}

void system_timer_init(void)
{
    systick_init(SYSTEM_TIMER_TICK_HZ, system_timer_tick_callback);
}

uint32_t system_timer_get_ms(void)
{
    return ticks_ms;
}