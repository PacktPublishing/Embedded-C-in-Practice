# Chapter 3 — Controlling Hardware Using Registers and GPIOs

This chapter introduces the first bare-metal firmware projects using direct register access and GPIOs.

## Projects

###  `01-led`

Minimal LED blink example using direct GPIO register access, with no abstraction over hardware.

###  `02-button-bounce`

Introduces the button, with polling and edge detection, but no debouncing. This version is left unfixed on purpose: run it to see the LED occasionally toggle twice from a single press.

If the bounce is not visible on your board, some Nucleo boards filter it partially through the on-board button circuit. Try a standalone button wired directly to the MCU without external filtering for the clearest effect.

###  `03-button-debounced`

Same driver files as `02-button-bounce`, with the debounce logic added to `main.c`.

###  `04-button-debounced-refactored`

The final, portable version of the same `03-button-debounced` project.