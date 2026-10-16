#include "systick.h"

#include "clock.h"
#include "stm32f4xx.h"

systick_status_t systick_init(uint32_t tick_hz)
{
    if (tick_hz == 0u)
    {
        return SYSTICK_ERROR_INVALID_ARG;
    }

    if ((CORE_CLOCK_HZ % tick_hz) != 0u)
    {
        return SYSTICK_ERROR_CONFIG;
    }

    if (SysTick_Config(CORE_CLOCK_HZ / tick_hz) != 0u)
    {
        return SYSTICK_ERROR_CONFIG;
    }

    return SYSTICK_OK;
}