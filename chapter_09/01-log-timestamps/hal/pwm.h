#ifndef PWM_H
#define PWM_H

#include <stdint.h>

typedef enum
{
    PWM_2
} pwm_instance_t;

typedef enum
{
    PWM_CHANNEL_1 = 1,
    PWM_CHANNEL_2 = 2,
    PWM_CHANNEL_3 = 3,
    PWM_CHANNEL_4 = 4
} pwm_channel_t;

typedef enum
{
    PWM_OK = 0,
    PWM_ERROR_INVALID_ARG,
    PWM_ERROR_NOT_INITIALIZED,
    PWM_FREQUENCY_MISMATCH,
    PWM_NO_ROUTE
} pwm_status_t;

typedef struct
{
    uint32_t frequency_hz;
} pwm_config_t;

pwm_status_t pwm_init(pwm_instance_t instance,
                      const pwm_config_t *config);
pwm_status_t pwm_enable_channel(pwm_instance_t instance,
                                pwm_channel_t channel);
void pwm_set_duty(pwm_instance_t instance,
                  pwm_channel_t channel, uint8_t percent);

#endif
