#include "delay.h"
#include <stdint.h>

void delay_short(void)
{
    for (volatile uint32_t i = 0; i < 20000; i++)
    {
    }
}

void delay_seconds(void)
{
    /* Calibrated by trial and error, exactly like delay_short(), and
     * just as non-portable: the actual duration depends on the core
     * clock and the optimization level. Adjust this constant on real
     * hardware until one call is close enough to one second. Chapter 6
     * replaces this with a hardware timer. */
    for (volatile uint32_t i = 0; i < 21000000; i++)
    {
    }
}
