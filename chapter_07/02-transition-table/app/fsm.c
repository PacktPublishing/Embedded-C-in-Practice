#include <stddef.h>

#include "fsm.h"
#include "limits.h"
#include "report.h"
#include "fan.h"
#include "log.h"

static fsm_state_t state = STATE_IDLE;

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

static void adjust_fan(uint32_t data)
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

/*
 * STATE_ANY marks a global transition: it matches in every state. It is
 * only used inside this table and is never assigned to state. The first
 * matching row wins, so global rows go after the specific ones.
 */
typedef void (*fsm_action_fn_t)(uint32_t data);

typedef struct
{
    fsm_state_t state;
    fsm_input_t input;
    fsm_action_fn_t action;
    fsm_state_t next;
} fsm_transition_t;

static const fsm_transition_t transitions[] =
{
    { STATE_IDLE, INPUT_START, start_monitoring, STATE_MONITOR },
    { STATE_MONITOR, INPUT_STOP, stop_monitoring, STATE_IDLE },
    { STATE_MONITOR, INPUT_OVER_THRESHOLD, raise_alert, STATE_ALERT },
    { STATE_ALERT, INPUT_WITHIN_LIMITS, clear_alert, STATE_MONITOR },
    { STATE_ALERT, INPUT_FAN_SPEED, adjust_fan, STATE_ALERT },
    { STATE_ANY, INPUT_BUTTON, panic_stop, STATE_IDLE },
};

/* An input with no matching row is ignored: these are the empty cells
 * of the state-input matrix. */
void fsm_dispatch(fsm_input_t input, uint32_t data)
{
    size_t count = sizeof(transitions) / sizeof(transitions[0]);

    for (size_t i = 0u; i < count; i++)
    {
        const fsm_transition_t *t = &transitions[i];

        if (((t->state == state) || (t->state == STATE_ANY)) &&
            (t->input == input))
        {
            t->action(data);
            state = t->next;
            return;
        }
    }
}

fsm_state_t fsm_state(void)
{
    return state;
}
