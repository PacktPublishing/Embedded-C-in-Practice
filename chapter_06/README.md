# Chapter 6 — Creating Non-Blocking Firmware with Timers

This chapter replaces every delay that measures time with hardware that counts it, builds a system time base, a debounce, and software timers on top of it, puts timers to work generating PWM and triggering ADC conversions, and lets the processor sleep between events.

## Projects

### `01-system-timer-time-base`
The starting point is the event-driven system from Chapter 5, without `delay_seconds()`. A system timer, split between `hal/systick.c` and `drivers/system_timer.c`, counts milliseconds, and the loop asks whether enough time has passed instead of waiting. The button and the console now answer on the next pass of the loop, while the temperature is still reported every second. Build with `-DSYSTEM_TIMER_START_MS=0xFFFFD8F0u` to start the tick 10 seconds before it wraps around.

### `02-one-shot-debounce`
The button gets a debounce that does not wait. `hal/timer.c` adds the general-purpose timers, and every edge restarts a one-shot TIM4, so the press is published only once the contact has stayed quiet for 20 ms. One press, one toggle.

### `03-software-timers`
The timestamp pattern moves into `app/soft_timer.c`, and two software timers share the same tick: the temperature report every second, and a status line every five seconds that also counts how many times the main loop ran.

### `04-timer-triggered-adc`
A timer schedules the ADC. Through `hal/adc.c`, TIM3 triggers a conversion on PA1 (ADC channel 1) every two seconds, and the end-of-conversion interrupt publishes a new event, `EVENT_SOIL_SAMPLE_READY`. A potentiometer on PA1 is enough to test it without a soil moisture probe.

### `05-pwm-fan`
`hal/pwm.c` builds PWM on top of `hal/timer.c`, and `drivers/fan.c` drives a 20 kHz signal on PB10 (TIM2 channel 3). Typing `0` to `9` in the terminal sets the duty cycle from 0 to 90 percent. The pin controls a 12 V fan through a logic-level MOSFET, never directly. Without a fan, an LED with a series resistor on PB10 shows the duty cycle as brightness.

### `06-sleep-until-event`
The complete system: the loop ends with `power_manager_sleep()`, and the processor sleeps until the next interrupt. The behavior is the same as in the previous project, but the status line shows the loop running about once per millisecond instead of continuously.