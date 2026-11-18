#ifndef CLOCK_H
#define CLOCK_H

/*
 * Clock frequencies used by the HAL.
 * Our project runs from the 16 MHz internal oscillator (HSI) selected at
 * reset, with the AHB, APB1, and APB2 prescalers at their reset value of 1.
 * If the clock tree changes, these values must change with it.
 */
#define CORE_CLOCK_HZ 16000000u
#define APB1_CLOCK_HZ 16000000u
#define APB2_CLOCK_HZ 16000000u

#endif
