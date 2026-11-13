#include "stack_monitor.h"
#include "cpu.h"

/*
 * _ebss is defined by the linker script: the first address after
 * .bss. The firmware does not use the heap, so the stack may grow down
 * to here.
 */
#define STACK_FILL_WORD   0xA5A5A5A5u
#define STACK_GUARD_WORDS 8u

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

bool stack_monitor_is_intact(void)
{
    const uint32_t *word = &_ebss;

    for (uint32_t i = 0u; i < STACK_GUARD_WORDS; i++)
    {
        if (word[i] != STACK_FILL_WORD)
        {
            return false;
        }
    }

    return true;
}

/*
 * Test only: recurses until the stack has gone past the bottom of its
 * region, overwriting the guard words and the top of .bss on the way.
 */
static void dive(void)
{
    volatile uint32_t block[16];

    for (uint32_t i = 0u; i < 16u; i++)
    {
        block[i] = 0u;
    }

    (void)block;

    if (cpu_get_stack_pointer() > ((uint32_t)&_ebss - 128u))
    {
        dive();
    }
}

void stack_monitor_force_overflow(void)
{
    dive();
}
