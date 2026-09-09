#include "stm32f401xe.h"

void SystemInit(void)
{
    /*
     * Enable the Cortex-M4 floating-point unit.
     *
     * Clock configuration will be introduced in a later chapter.
     */
    SCB->CPACR |= (3UL << 20) | (3UL << 22);

    __DSB();
    __ISB();
}
