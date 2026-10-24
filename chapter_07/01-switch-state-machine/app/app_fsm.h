#ifndef APP_FSM_H
#define APP_FSM_H

#include <stdint.h>

typedef enum
{
    STATE_IDLE,
    STATE_MONITOR,
    STATE_ALERT
} app_state_t;

typedef enum
{
    INPUT_START,
    INPUT_STOP,
    INPUT_BUTTON,
    INPUT_FAN_SPEED,
    INPUT_OVER_THRESHOLD,
    INPUT_WITHIN_LIMITS
} app_input_t;

void app_fsm_dispatch(app_input_t input, uint32_t data);
app_state_t app_fsm_state(void);

#endif // APP_FSM_H
