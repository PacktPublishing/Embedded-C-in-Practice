#include "led.h"
#include "button.h"

int main(void)
{
    led_init();
    button_init();

    bool previous = false;

    while (1)
    {
        bool current = button_is_pressed();

        if (current && !previous)
        {
            led_toggle();
        }

        previous = current;
    }
}