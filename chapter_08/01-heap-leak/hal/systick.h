#ifndef HAL_SYSTICK_H
#define HAL_SYSTICK_H

#include <stdint.h>

typedef enum
{
    SYSTICK_OK = 0,
    SYSTICK_ERROR_INVALID_ARG,
    SYSTICK_ERROR_CONFIG
} systick_status_t;

systick_status_t systick_init(uint32_t tick_hz);

#endif // HAL_SYSTICK_H