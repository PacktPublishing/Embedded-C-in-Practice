# Chapter 9 — Debugging, Testing and Validation on Real Hardware

This chapter observes the firmware from the outside while it runs, instead of relying on what the firmware chooses to report about itself. The three projects start from the complete system of Chapter 8 and add the three things that chapter left missing: a log that says when, a pin that measures real timing without disturbing it, and a set of self-checks that can be rerun after every change. The debugger techniques of the chapter, breakpoints, watchpoints, and stepping, need no code of their own: they work on any of these projects, and on the Chapter 8 projects as well.

## Projects

### `01-log-timestamps`

The complete system from Chapter 8 with an extended log module. Every line now begins with the millisecond count from `system_timer_get_ms()`, so a run reads as a timeline instead of a list of messages. `drivers/log.h` adds a fourth level, `LOG_DEBUG`, and a compile-time threshold, `LOG_LEVEL`, with one macro per level: `log_debug()`, `log_info()`, `log_warning()`, and `log_error()`. A call below the threshold is removed by the preprocessor, so it costs nothing in Flash and nothing at runtime, which matters because the projects build at `-O0`, where a runtime comparison would not be optimized away. `read_temperature()` in `main.c` carries two `log_debug()` calls. Build once with `make` and once with `make LOG_LEVEL=LOG_LEVEL_DEBUG`, and compare the output of `arm-none-eabi-size` to see what the threshold removes: the default build contains neither the calls nor their strings. The call sites in `app/fsm.c`, `app/report.c`, and `main.c` now use the macros instead of calling `log_print()` directly.

### `02-debug-pin`

Adds a debug output on PB4, driven high as the first statement of the main loop and low just before the processor goes to sleep. With an oscilloscope or a logic analyzer on that pin, the width of a pulse is the duration of one loop pass, the gaps are the time the processor stays idle, and the wider pulse once per second is the cost of the periodic sensor reading, which includes the I2C transaction and one log line. Each edge is one write to `BSRR` through `gpio_write_pin()`, a handful of instructions, so the measurement disturbs the timing far less than a log line would. Move the two calls around any other region to measure it the same way.

### `03-smoke-tests`

Adds a set of self-checks that run on the real hardware and report a uniform verdict. Type `t` on the console to run all of them: the sensor check performs one reading, the probe check verifies that an analog sample has arrived and falls inside the range of the converter, the tick check verifies that the millisecond counter advances, and the queue check puts a known event into a local queue and takes it out again. Each result is logged as `PASS` or `FAIL`, so two runs can be compared at a glance after a change. The command joins the diagnostic commands of the previous chapter, `m` and `x`, in `handle_command()`, outside the state machine.

## Console commands

The commands available in all three projects:

```text
s        start monitoring
e        stop monitoring
0-9      fan speed, in alert only
m        bytes of stack never used since reset
x        force a stack overflow, to see the blink code
t        run the smoke tests (03-smoke-tests only)
```

## Building

Each project builds on its own:

```text
cd 01-log-timestamps
make
make flash
```

The serial terminal runs at 115200 baud.
