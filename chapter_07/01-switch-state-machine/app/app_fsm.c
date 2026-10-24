#include "app_fsm.h"
#include "limits.h"
#include "report.h"
#include "fan.h"
#include "log.h"

static app_state_t state = STATE_IDLE;

static void start_monitoring(uint32_t data)
{
    (void)data;

    limits_reset();
    log_print(LOG_INFO, "monitor: started");
}

static void stop_monitoring(uint32_t data)
{
    (void)data;

    log_print(LOG_INFO, "monitor: stopped");
}

static void raise_alert(uint32_t data)
{
    (void)data;

    fan_set_speed(100u);
    log_print(LOG_WARNING, "alert: reading over limit");
}

static void clear_alert(uint32_t data)
{
    (void)data;

    fan_set_speed(0u);
    log_print(LOG_INFO, "alert: cleared");
}

static void set_fan_speed(uint32_t data)
{
    fan_set_speed((uint8_t)data);
    report_fan_speed((uint8_t)data);
}

static void panic_stop(uint32_t data)
{
    (void)data;

    fan_set_speed(0u);
    log_print(LOG_WARNING, "panic: stopped");
}

static void handle_idle(app_input_t input, uint32_t data)
{
    switch (input)
    {
    case INPUT_START:
        start_monitoring(data);
        state = STATE_MONITOR;
        break;
    default:
        break;
    }
}

static void handle_monitor(app_input_t input, uint32_t data)
{
    switch (input)
    {
    case INPUT_STOP:
        stop_monitoring(data);
        state = STATE_IDLE;
        break;
    case INPUT_OVER_THRESHOLD:
        raise_alert(data);
        state = STATE_ALERT;
        break;
    default:
        break;
    }
}

static void handle_alert(app_input_t input, uint32_t data)
{
    switch (input)
    {
    case INPUT_FAN_SPEED:
        set_fan_speed(data);
        break;
    case INPUT_WITHIN_LIMITS:
        clear_alert(data);
        state = STATE_MONITOR;
        break;
    default:
        break;
    }
}

void app_fsm_dispatch(app_input_t input, uint32_t data)
{
    /* The emergency stop is a global transition: it applies in every
     * state, so it is handled before looking at the current one. */
    if (input == INPUT_BUTTON)
    {
        panic_stop(data);
        state = STATE_IDLE;
        return;
    }

    switch (state)
    {
    case STATE_IDLE:
        handle_idle(input, data);
        break;
    case STATE_MONITOR:
        handle_monitor(input, data);
        break;
    case STATE_ALERT:
        handle_alert(input, data);
        break;
    default:
        break;
    }
}

app_state_t app_fsm_state(void)
{
    return state;
}
