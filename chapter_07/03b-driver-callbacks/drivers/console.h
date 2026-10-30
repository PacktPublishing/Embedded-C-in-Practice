#ifndef DRIVERS_CONSOLE_H
#define DRIVERS_CONSOLE_H

#include <stdint.h>

typedef void (*console_callback_t)(uint8_t byte);

void console_set_rx_callback(console_callback_t callback);

void console_init(void);

#endif /* DRIVERS_CONSOLE_H */