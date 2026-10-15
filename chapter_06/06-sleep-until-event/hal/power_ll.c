
#include "power_ll.h"

#include "stm32f4xx.h"

void power_ll_init(void)
{
    DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP;
}

void power_ll_sleep(void)
{
    __WFI();
}