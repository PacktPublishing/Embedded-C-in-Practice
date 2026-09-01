#include "led.h"

int main(void)
{
    led_init();
    led_set(true);

    while (1)
    {
    }
}