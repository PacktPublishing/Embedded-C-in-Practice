#include "soft_timer.h"

/*
 * Anchors the schedule to the current tick. A timer declared with
 * last_run at zero works as long as the tick also starts at zero, but
 * starting it explicitly keeps it correct with SYSTEM_TIMER_START_MS.
 */
void soft_timer_start(soft_timer_t *timer, uint32_t now)
{
    timer->last_run = now;
}

/*
 * Re-arming by adding the interval keeps deadlines on their original
 * schedule. If the loop falls behind by several intervals, the timer
 * reports one expiry per call until it catches up.
 */
bool soft_timer_expired(soft_timer_t *timer, uint32_t now)
{
    if ((now - timer->last_run) < timer->interval_ms)
    {
        return false;
    }

    timer->last_run += timer->interval_ms;
    return true;
}
