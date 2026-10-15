
#include "systick.h"

#include "clock.h"
#include "stm32f4xx.h"

static systick_callback_t tick_callback;

systick_status_t systick_init(uint32_t tick_hz,
                              systick_callback_t callback)
{
    if ((tick_hz == 0u) || (callback == 0))
    {
        return SYSTICK_ERROR_INVALID_ARG;
    }

    if ((CORE_CLOCK_HZ % tick_hz) != 0u)
    {
        return SYSTICK_ERROR_CONFIG;
    }

    uint32_t ticks = CORE_CLOCK_HZ / tick_hz;

    if (ticks == 0u)
    {
        return SYSTICK_ERROR_CONFIG;
    }

    tick_callback = callback;

    if (SysTick_Config(ticks) != 0u)
    {
        tick_callback = 0;
        return SYSTICK_ERROR_CONFIG;
    }

    return SYSTICK_OK;
}

void SysTick_Handler(void)
{
    if (tick_callback != 0)
    {
        tick_callback();
    }
}