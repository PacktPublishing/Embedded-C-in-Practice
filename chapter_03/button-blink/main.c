#include <stdbool.h>
#include <stdint.h>

#include "button.h"
#include "led.h"

static void delay_short(void)
{
    for (volatile uint32_t i = 0; i < 50000U; ++i)
    {
        __asm volatile ("nop");
    }
}

int main(void)
{
    led_init();
    button_init();

    bool stable = false;
    bool candidate = false;
    uint32_t count = 0U;

    while (1)
    {
        bool raw = button_is_pressed();

        count = (raw == candidate) ? (count + 1U) : 0U;
        candidate = raw;

        if ((count >= 4U) && (candidate != stable))
        {
            stable = candidate;

            if (stable)
            {
                led_toggle();
            }
        }

        delay_short();
    }
}
