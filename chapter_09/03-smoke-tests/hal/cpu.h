#ifndef HAL_CPU_H
#define HAL_CPU_H

#include <stdint.h>

uint32_t cpu_get_stack_pointer(void);
void cpu_disable_interrupts(void);

#endif // HAL_CPU_H
