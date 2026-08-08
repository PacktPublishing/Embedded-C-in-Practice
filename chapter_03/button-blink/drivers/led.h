#ifndef LED_H
#define LED_H

#include <stdbool.h>

void led_init(void);
void led_set(bool state);
void led_toggle(void);

#endif
