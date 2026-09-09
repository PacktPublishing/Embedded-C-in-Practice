#ifndef APP_DELAY_H
#define APP_DELAY_H

/* From Chapter 3: a short busy-wait, calibrated for button debounce. */
void delay_short(void);

/* From Chapter 4: the same technique, recalibrated for a much longer
 * wait and given its own name because a full second and a debounce
 * interval have nothing in common beyond the busy-wait trick itself. */
void delay_seconds(void);

#endif
