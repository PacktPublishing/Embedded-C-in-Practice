# Chapter 4 — Designing Reusable Peripheral Drivers

This chapter turns register-level code into reusable drivers, fills the `hal/` folder, and leaves the microcontroller for the first time to talk to external chips over I2C and SPI.

## Projects

### `01-hal-gpio`
GPIO abstraction in `hal/`. `led.c` and `button.c` are reduced to one line each, same behavior as Chapter 3.

### `02-uart-multi-instance`
UART driver built around a handle, two instances configured (debug link and external module). Blocking and polled on purpose, fixed in Chapter 5.

### `03-imu-i2c`
I2C bus driver plus a driver for the ST LSM6DSOX motion sensor, following init, config, read. `main.c` reads acceleration and rotation without knowing what I2C is.
Wiring: SCL→PB8, SDA→PB9. Address is 0x6A or 0x6B depending on the sensor's SA0 pin.

### `04-spi-flash`
SPI bus driver plus a minimal external flash driver, verified through its JEDEC identity. Final state of the chapter.
Wiring: SCK→PB3, MISO→PB4, MOSI→PB5, CS→PB6 (non-default pins, the default set collides with the LED on PA5).
On the flash module, DI is the chip's input (→ MOSI) and DO is its output (→ MISO): swapping them gives no bus error, just a failed identity check.