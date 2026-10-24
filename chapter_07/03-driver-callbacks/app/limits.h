#ifndef APP_LIMITS_H
#define APP_LIMITS_H

#include <stdbool.h>
#include <stdint.h>

void limits_reset(void);
void limits_check_temperature(float celsius);
void limits_check_probe(uint16_t raw);
bool limits_exceeded(void);

#endif // APP_LIMITS_H
