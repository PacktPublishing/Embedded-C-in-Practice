#include "delay.h"
#include <stdint.h>

/*
 * Both delays are calibrated by trial and error on real hardware.
 * The actual duration depends on the core clock and optimization level.
 */

void delay_short(void)
{
    for (volatile uint32_t i = 0; i < 20000; i++)
    {
    }
}

void delay_seconds(void)
{
    for (volatile uint32_t i = 0; i < 21000000; i++)
    {
    }
}
