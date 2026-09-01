#include <stdint.h>
#include "led.h"
#include "button.h"

static void delay_short(void)
{
    for (volatile uint32_t i = 0; i < 20000; i++)
    {
    }
}

int main(void)
{
    led_init();
    button_init();

    bool stable = false;
    bool candidate = false;
    uint32_t count = 0;

    while (1)
    {
        bool raw = button_is_pressed();
        count = (raw == candidate) ? (count + 1) : 0;
        candidate = raw;

        if ((count >= 3) && (candidate != stable))
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