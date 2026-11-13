# Chapter 8 — Managing Memory in Embedded Systems

This chapter follows the firmware's RAM byte by byte: the static data placed by the linker, the stack that grows down from the top of RAM, and the heap that would grow up from the end of `.bss`, with nothing between the two. The first two projects are a small standalone program that shows how a heap leak reaches the stack, and how a static pool changes the failure. The last three start from the complete system of Chapter 7 and teach it to measure and guard its own stack.

## Projects

### `01-heap-leak`
A standalone program, separate from the system of the previous chapters. Every 100 ms it logs a status line allocated with `malloc()` and toggles the LED. The line reports its own address, and the caller never frees it. The addresses climb by 72 bytes per line toward the top of RAM, and after a little over two minutes a new block overlaps the stack frames of the running code: the last line comes out garbled, the processor faults into `Default_Handler`, and the LED stops. `malloc()` never returns `NULL`, because the `_sbrk()` linked with `--specs=nosys.specs` does not check the stack. The drivers are reused unchanged from Chapter 7, which is possible because the callbacks of that chapter removed their dependency on the application.

### `02-static-pool`
The same program with the line taken from a static pool of eight 64-byte blocks (`app/pool.c`) and returned with `pool_release()` after logging. Every line reports the same address. Remove the call to `pool_release()` in `report_status()` to see the leak fail in a different way: after eight lines, the pool is empty, `pool_alloc()` returns `NULL`, and every following cycle logs an error while the LED keeps blinking.

### `03-stack-watermark`
The complete system from Chapter 7. `drivers/stack_monitor.c` fills the free RAM between `_ebss` and the stack pointer with a known pattern as the first statement of `main()`, and type `m` on the console to log how many bytes of stack have never been used since reset. `hal/cpu.c` wraps the CMSIS-Core intrinsic that reads the stack pointer. No change to the linker script or the startup code is needed.

### `04-stack-canary`
Adds a guard: the main loop checks the eight lowest words of the fill on every pass, and if any of them has changed, `drivers/blink_code.c` disables interrupts and blinks the LED in groups of two, forever. The delay reads the SysTick `COUNTFLAG` bit through `systick_has_wrapped()`, so it works with interrupts disabled. Type `x` to run a test that recurses until the stack passes the bottom of its region, overwriting the guard and the top of `.bss` on purpose, and watch the blink code appear. Reset the board to recover.

### `05-static-checks`
Adds two compile-time checks with `_Static_assert`: the size of `event_t`, which depends on how the toolchain sizes an enumeration, and the minimum size of the event queue, which keeps one slot empty. Change `EVENT_QUEUE_SIZE` to 1, or the type of `event_t.data`, to see the build stop with the message written next to the assumption.
