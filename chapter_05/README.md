# Chapter 5 — Handling Interrupts and Asynchronous Events

This chapter replaces waiting with hardware that announces events, teaches interrupt handlers to signal instead of process, and turns the firmware into an event-driven application that reacts to both a button and the PC.

## Projects

### `01-polling-button`
The starting point: the boring `main()` from Chapter 4 with the button from Chapter 3 polled inside the loop. The temperature is still reported every second, but most presses are lost while the loop sits in `delay_seconds()`.

### `02-interrupt-button`
The button moves to an interrupt, with EXTI and NVIC configured through a new `hal/exti.c`. The ISR toggles the LED directly, so a press is served as soon as the processor can take the interrupt, whatever the main loop is doing.

### `03-interrupt-button-shared-flag`
The ISR stops acting and only signals, raising a `volatile` flag that the main loop consumes. A press is no longer missed, only handled at the next pass of the loop, but presses that arrive close together merge into one.

### `03b-interrupt-button-shared-flag`
A variant of the previous project that replaces the flag with a counter, protected by a critical section, so that every press is remembered and handled.

### `04-event-driven-button-console`
The complete system: the button and a UART console produce events into a circular queue, and the main loop consumes them. A press toggles the LED, while `s` and `e` typed in the terminal start and stop the BME280 acquisition, the first event in the book to carry a payload.