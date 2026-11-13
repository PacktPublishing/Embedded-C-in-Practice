
#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include <stdint.h>

void system_timer_init(void);
uint32_t system_timer_get_ms(void);

#endif // SYSTEM_TIMER_H