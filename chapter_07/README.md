# Chapter 7 — Implementing State Machines for Embedded Systems

This chapter gives the application logic an explicit structure: the modes that were hidden in flags become the states of a finite state machine, the rules become transitions, and function pointers let the machine live in a table and free the drivers from the application above them.

## Projects

### `01-switch-state-machine`
The starting point is the system from Chapter 6, with a new requirement: while monitoring, a temperature or a probe value over its limit raises an alert. `app/fsm.c` implements the machine with an `enum` and a `switch`, with three states, `IDLE`, `MONITOR`, and `ALERT`. `handle_event()` in `main.c` only translates events into inputs, and `app/limits.c` turns readings into a single over-limit input, with hysteresis. Type `s` to start monitoring and `e` to stop it. In an alert, the fan runs at full speed, `0` to `9` adjust it, and `e` is ignored. The button is an emergency stop that returns to `IDLE` from any state. A potentiometer on PA1 is enough to cross the probe limits (above 3000 to enter an alert, below 2800 to leave it). The status line of Chapter 6 is gone, and the button no longer toggles the LED.

### `02-transition-table`
The same behavior, with the machine written as a `static const` table of transitions and a generic dispatcher that calls each action through a function pointer. The emergency stop is a single row that matches any state. Only `app/fsm.c` and `app/fsm.h` change.

### `03-driver-callbacks`
The complete system. The button, console, and probe drivers no longer include `app/event_queue.h`: each one calls a function pointer registered by `main.c`, which publishes the event. The LED shows the state: off in `IDLE`, a one-second period in `MONITOR`, and a 250 ms period in `ALERT`. The processor still sleeps between events.

### `03b-driver-callbacks`
A worked first step of the exercise at the end of the callbacks section, with the same behavior as project 03. The vector name `TIM4_IRQHandler` moves from `drivers/button.c` into `hal/timer.c`. The HAL acknowledges the update interrupt and calls a callback that the driver registers with `timer_set_update_callback()`, so the button driver no longer contains any vector name for its debounce timer. The handlers for `EXTI15_10_IRQHandler`, `USART2_IRQHandler`, and `ADC_IRQHandler` are still in the drivers, and moving them the same way is the exercise.
