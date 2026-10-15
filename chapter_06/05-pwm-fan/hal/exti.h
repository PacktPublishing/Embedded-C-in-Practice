#ifndef HAL_EXTI_H
#define HAL_EXTI_H

#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"

typedef enum
{
    EXTI_TRIGGER_RISING,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH
} exti_trigger_t;

typedef struct
{
    exti_trigger_t trigger;
    uint8_t priority;
} exti_config_t;

void exti_configure(gpio_port_t port, uint8_t pin,
                    const exti_config_t *config);

bool exti_is_pending(uint8_t pin);
void exti_clear_pending(uint8_t pin);

#endif // HAL_EXTI_H