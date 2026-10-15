#include "power.h"

#include "stm32f4xx.h"

/*
 * In sleep mode, the core clock stops, and on STM32 devices the debugger
 * can lose its connection. DBG_SLEEP keeps the debug logic clocked while
 * the core sleeps, at the cost of a slightly higher sleep current. The
 * setting only matters while a debugger is attached.
 */
void power_init(void)
{
    DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP;
}
