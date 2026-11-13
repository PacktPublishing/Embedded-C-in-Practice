#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    GPIO_A,
    GPIO_B,
    GPIO_C,
    GPIO_D
} gpio_port_t;

typedef enum
{
    GPIO_DIRECTION_INPUT,
    GPIO_DIRECTION_OUTPUT
} gpio_direction_t;

typedef enum
{
    GPIO_PUSH_PULL = 0,
    GPIO_OPEN_DRAIN = 1
} gpio_output_type_t;

typedef enum
{
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP = 1,
    GPIO_PULL_DOWN = 2
} gpio_pull_t;

typedef struct
{
    gpio_direction_t direction;
    gpio_output_type_t output_type;
    gpio_pull_t pull;
} gpio_config_t;

void gpio_configure(gpio_port_t port, uint8_t pin,
                    const gpio_config_t *config);
void gpio_configure_alternate(gpio_port_t port, uint8_t pin,
                              uint8_t function, gpio_output_type_t type);
void gpio_configure_analog(gpio_port_t port, uint8_t pin);
void gpio_write_pin(gpio_port_t port, uint8_t pin, bool level);
bool gpio_read_pin(gpio_port_t port, uint8_t pin);

#endif // GPIO_H
