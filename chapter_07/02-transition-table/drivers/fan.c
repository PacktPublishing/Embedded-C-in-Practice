#include "fan.h"

#include "pwm.h"

/*
 * Channel 3 of PWM_2 comes out on PB10. The pin drives the fan through a
 * logic-level N-channel MOSFET, never directly. 20 kHz keeps the
 * switching above the range most people can hear.
 */
#define FAN_PWM PWM_2
#define FAN_PWM_CHANNEL PWM_CHANNEL_3
#define FAN_PWM_HZ 20000u

void fan_init(void)
{
    const pwm_config_t config = { .frequency_hz = FAN_PWM_HZ };

    pwm_init(FAN_PWM, &config);
    pwm_enable_channel(FAN_PWM, FAN_PWM_CHANNEL);
}

void fan_set_speed(uint8_t percent)
{
    pwm_set_duty(FAN_PWM, FAN_PWM_CHANNEL, percent);
}
