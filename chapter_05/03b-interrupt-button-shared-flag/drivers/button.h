#ifndef DRIVERS_BUTTON_H
#define DRIVERS_BUTTON_H

#include <stdbool.h>

void button_init(void);
bool button_is_pressed(void);
void button_enable_interrupt(void);
bool button_consume_press(void);

#endif // DRIVERS_BUTTON_H
