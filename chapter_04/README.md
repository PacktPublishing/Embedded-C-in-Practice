# Chapter 4 — Designing Reusable Peripheral Drivers

This chapter turns register-level code into reusable drivers, fills the `hal/` folder for real, and leaves the microcontroller for the first time to talk to an external chip over I2C.

## Projects

### `01-hal-gpio`
The GPIO abstraction on its own: one function to configure a pin, one to drive it. The LED is toggled directly through the HAL, no driver in between.

### `02-hal-uart`
The UART driver with a hardware-independent instance and an internally managed handle. `main()` writes to `UART_2` directly, same pattern as the GPIO project.

### `03-i2c-sensor-log`
The complete system: a BME280 read over I2C, reported through the log module over UART, with the LED as an error indicator. Every layer from this chapter working together.