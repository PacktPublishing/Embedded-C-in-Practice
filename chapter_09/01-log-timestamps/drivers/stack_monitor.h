#ifndef DRIVERS_STACK_MONITOR_H
#define DRIVERS_STACK_MONITOR_H

#include <stdbool.h>
#include <stdint.h>

void stack_monitor_init(void);
uint32_t stack_monitor_unused_bytes(void);
bool stack_monitor_is_intact(void);
void stack_monitor_force_overflow(void);

#endif // DRIVERS_STACK_MONITOR_H
