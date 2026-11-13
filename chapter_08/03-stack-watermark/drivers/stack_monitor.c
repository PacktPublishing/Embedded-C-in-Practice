#include "stack_monitor.h"
#include "cpu.h"

/*
 * _ebss is defined by the linker script: the first address after
 * .bss. The firmware does not use the heap, so the stack may grow down
 * to here.
 */
#define STACK_FILL_WORD 0xA5A5A5A5u

extern uint32_t _ebss;

void stack_monitor_init(void)
{
    uint32_t *word = &_ebss;
    uint32_t top = cpu_get_stack_pointer();

    while ((uint32_t)word < top)
    {
        *word = STACK_FILL_WORD;
        word++;
    }
}

uint32_t stack_monitor_unused_bytes(void)
{
    const uint32_t *word = &_ebss;
    uint32_t unused = 0u;

    while (*word == STACK_FILL_WORD)
    {
        unused += sizeof(uint32_t);
        word++;
    }

    return unused;
}
