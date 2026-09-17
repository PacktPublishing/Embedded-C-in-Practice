## Driver Design: Stateless vs Stateful

Drivers hide hardware-specific details behind small, purpose-oriented APIs.

Board-specific mappings and fixed peripheral configurations are kept inside
the corresponding drivers rather than exposed to application code.

Not every driver needs a handle. A handle is useful when the driver must retain
state associated with a specific device instance. The LED, button, and log
modules have only fixed board-specific configuration, so their APIs remain
stateless. The BME280 driver, instead, retains calibration data, configuration,
and device information, making a device handle appropriate.

The rule is simple:

> Don't introduce a handle just because it's a driver. Introduce one when the
> driver needs to retain per-device state between calls.

### Driver State

1. **BME280** → per-device state → dedicated handle (`bme280_t`)
   - I2C handle
   - Device address
   - Calibration data
   - Configuration
   - Initialization state

2. **LED** → stateless → no handle
   - `LED_PORT = GPIO_A`
   - `LED_PIN = 5`
   - `LED_ACTIVE_HIGH = 1`

3. **Button** → stateless → no handle
   - `BUTTON_PORT = GPIO_C`
   - `BUTTON_PIN = 13`
   - `BUTTON_ACTIVE_LOW = 1`

4. **Log** → stateless → no handle
   - `LOG_UART = UART_2`
   - `LOG_UART_BAUDRATE = 115200`