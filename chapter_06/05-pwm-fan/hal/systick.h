
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

typedef void (*systick_callback_t)(void);

typedef enum
{
    SYSTICK_OK = 0,
    SYSTICK_ERROR_INVALID_ARG,
    SYSTICK_ERROR_CONFIG
} systick_status_t;

systick_status_t systick_init(uint32_t tick_hz,
                              systick_callback_t callback);

#endif // SYSTICK_H