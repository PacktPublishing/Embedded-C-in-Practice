#include "pwm.h"

#include <stdbool.h>
#include <stddef.h>

#include "gpio.h"
#include "timer.h"

/* One count per percentage point of duty cycle. */
#define PWM_STEPS 100u

typedef struct
{
    timer_instance_t timer;
    uint32_t frequency_hz;
    bool initialized;
} pwm_handle_t;

typedef struct
{
    gpio_port_t port;
    uint8_t pin;
    uint8_t function;
} pwm_route_t;

static pwm_handle_t pwm2_handle = { .timer = TIMER_2 };

/*
 * Pin chosen for each channel of PWM_2 on our board. A timer channel can
 * reach more than one pin, and this table fixes one, like the pins of
 * UART_2 inside uart_init(). Channels 2 and 4 are not routed.
 */
static const pwm_route_t pwm2_routes[4] =
{
    { GPIO_A, 5, 1 },
    { GPIO_A, 0, 0 },
    { GPIO_B, 10, 1 },
    { GPIO_A, 0, 0 }
};

static pwm_handle_t *get_handle(pwm_instance_t instance)
{
    return (instance == PWM_2) ? &pwm2_handle : NULL;
}

static const pwm_route_t *get_route(pwm_instance_t instance,
                                    pwm_channel_t channel)
{
    if ((instance != PWM_2) || (channel < PWM_CHANNEL_1) ||
        (channel > PWM_CHANNEL_4))
    {
        return NULL;
    }

    const pwm_route_t *route = &pwm2_routes[(uint32_t)channel - 1u];

    return (route->function == 0u) ? NULL : route;
}

pwm_status_t pwm_init(pwm_instance_t instance,
                      const pwm_config_t *config)
{
    pwm_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || (config == NULL) ||
        (config->frequency_hz == 0u))
    {
        return PWM_ERROR_INVALID_ARG;
    }

    /*
     * The channels of one timer share its counter, and so its period.
     * A second caller asking for a different frequency gets an error
     * instead of silently running at the first caller's frequency.
     */
    if (handle->initialized)
    {
        return (config->frequency_hz == handle->frequency_hz)
                   ? PWM_OK
                   : PWM_FREQUENCY_MISMATCH;
    }

    const timer_config_t timer_config =
    {
        .mode = TIMER_MODE_PERIODIC,
        .tick_hz = config->frequency_hz * PWM_STEPS,
        .period_ticks = PWM_STEPS
    };

    if (timer_init(handle->timer, &timer_config) != TIMER_OK)
    {
        return PWM_ERROR_INVALID_ARG;
    }

    timer_start(handle->timer);

    handle->frequency_hz = config->frequency_hz;
    handle->initialized = true;

    return PWM_OK;
}

pwm_status_t pwm_enable_channel(pwm_instance_t instance,
                                pwm_channel_t channel)
{
    pwm_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return PWM_ERROR_NOT_INITIALIZED;
    }

    const pwm_route_t *route = get_route(instance, channel);

    if (route == NULL)
    {
        return PWM_NO_ROUTE;
    }

    gpio_configure_alternate(route->port, route->pin,
                             route->function, GPIO_PUSH_PULL);
    timer_configure_compare(handle->timer, (timer_channel_t)channel);

    return PWM_OK;
}

void pwm_set_duty(pwm_instance_t instance,
                  pwm_channel_t channel, uint8_t percent)
{
    pwm_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return;
    }

    if (percent > 100u)
    {
        percent = 100u;
    }

    uint32_t period = timer_get_period_ticks(handle->timer);

    timer_set_compare(handle->timer, (timer_channel_t)channel,
                      (period * percent) / 100u);
}
