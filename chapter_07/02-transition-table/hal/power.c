
#include "power.h"

#include "stm32f4xx.h"

void power_init(void)
{
    DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP;
}

void power_sleep(void)
{
    __WFI();
}