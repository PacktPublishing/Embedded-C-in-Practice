#ifndef DRIVERS_LED_H
#define DRIVERS_LED_H

#include <stdbool.h>

void led_init(void);
void led_set(bool status);
void led_toggle(void);

#endif // DRIVERS_LED_H
