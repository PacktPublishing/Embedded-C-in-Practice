#include "cpu.h"

#include "stm32f4xx.h"

uint32_t cpu_get_stack_pointer(void)
{
    return __get_MSP();
}

void cpu_disable_interrupts(void)
{
    __disable_irq();
}
