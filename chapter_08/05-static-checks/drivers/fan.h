#ifndef FAN_H
#define FAN_H

#include <stdint.h>

void fan_init(void);
void fan_set_speed(uint8_t percent);

#endif
