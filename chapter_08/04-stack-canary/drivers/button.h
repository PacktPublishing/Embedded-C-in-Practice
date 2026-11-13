#ifndef DRIVERS_BUTTON_H
#define DRIVERS_BUTTON_H

#include <stdbool.h>

typedef void (*button_callback_t)(void);

void button_init(void);
bool button_is_pressed(void);
void button_set_press_callback(button_callback_t callback);
void button_enable_interrupt(void);

#endif // DRIVERS_BUTTON_H
