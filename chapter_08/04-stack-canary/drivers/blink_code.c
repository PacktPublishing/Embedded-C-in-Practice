#include "blink_code.h"
#include "led.h"
#include "cpu.h"
#include "systick.h"

#define BLINK_ON_MS    200u
#define BLINK_OFF_MS   300u
#define BLINK_PAUSE_MS 1500u

/*
 * Busy-waits on the SysTick counter, which keeps counting with
 * interrupts disabled. Requires system_timer_init() to have run.
 */
static void wait_ms(uint32_t ms)
{
    while (ms > 0u)
    {
        if (systick_has_wrapped())
        {
            ms--;
        }
    }
}

void blink_code_halt(uint8_t code)
{
    cpu_disable_interrupts();

    for (;;)
    {
        for (uint8_t i = 0u; i < code; i++)
        {
            led_set(true);
            wait_ms(BLINK_ON_MS);
            led_set(false);
            wait_ms(BLINK_OFF_MS);
        }

        wait_ms(BLINK_PAUSE_MS);
    }
}
