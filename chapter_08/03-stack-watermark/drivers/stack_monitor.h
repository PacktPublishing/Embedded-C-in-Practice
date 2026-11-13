#ifndef DRIVERS_STACK_MONITOR_H
#define DRIVERS_STACK_MONITOR_H

#include <stdint.h>

void stack_monitor_init(void);
uint32_t stack_monitor_unused_bytes(void);

#endif // DRIVERS_STACK_MONITOR_H
