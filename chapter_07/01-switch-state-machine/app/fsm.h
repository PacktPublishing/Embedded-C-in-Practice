#ifndef FSM_H
#define FSM_H

#include <stdint.h>

typedef enum
{
    STATE_IDLE,
    STATE_MONITOR,
    STATE_ALERT
} fsm_state_t;

typedef enum
{
    INPUT_START,
    INPUT_STOP,
    INPUT_BUTTON,
    INPUT_FAN_SPEED,
    INPUT_OVER_THRESHOLD,
    INPUT_WITHIN_LIMITS
} fsm_input_t;

void fsm_dispatch(fsm_input_t input, uint32_t data);
fsm_state_t fsm_state(void);

#endif // FSM_H
