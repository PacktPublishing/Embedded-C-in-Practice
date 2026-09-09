# Chapter 4 — Designing Reusable Peripheral Drivers

This chapter turns register-level code into reusable drivers, fills the `hal/` folder for real, and leaves the microcontroller for the first time to talk to an external chip over I2C.

## Projects

### `01-hal-gpio`
GPIO abstraction in `hal/`, built around a hardware-independent port identifier resolved internally rather than a raw peripheral pointer. `led.c` and `button.c` are reduced to one line each, same behavior as Chapter 3, now expressed through `gpio_write_pin()` and `gpio_read_pin()` instead of `BSRR` and `IDR` directly.

### `02-uart-multi-instance`
UART driver built around a handle and a hardware-independent instance, the same discipline the GPIO abstraction gets retrofitted with later in the chapter. Two instances configured to prove the point. Blocking and polled on purpose, fixed in Chapter 5.

### `03-i2c-bme280`
I2C bus driver, plus a full driver for the Bosch BME280 environmental sensor, following the init, configure, read pattern. Includes the sensor's identity check, its register-based configuration, a single burst read of the eight data registers, and the Bosch compensation formulas the book keeps out of the text. This is also where the GPIO abstraction gets its hardware-independent instance, retrofitted after the pattern has already shown up twice.
Wiring: SCL→PB8, SDA→PB9, alternate function 4, open-drain. Address is 0x76 or 0x77 depending on the sensor's SDO pin.

### `04-decoupled-application`
The chapter's final state. `main.c` reads the sensor once a second and reports the temperature without containing a single line about I2C, UART, or any register. The debug module owns the UART handle entirely, the LED doubles as an error indicator when the sensor is not found, and `app/` holds exactly one function.
Wiring: same as `03-i2c-bme280`, plus USART2 TX→PA2, RX→PA3 for the debug link to the host.