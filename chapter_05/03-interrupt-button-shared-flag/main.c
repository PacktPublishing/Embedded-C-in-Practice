#include "led.h"
#include "bme280.h"
#include "log.h"
#include "report_temperature.h"
#include "delay.h"
#include "button.h"

#define BME280_I2C_ADDR 0x76

static bme280_t sensor;

static const bme280_config_t sensor_config =
{
    .temperature_oversampling = BME280_OVERSAMPLING_X1,
    .pressure_oversampling = BME280_OVERSAMPLING_X1,
    .humidity_oversampling = BME280_OVERSAMPLING_X1,
    .mode = BME280_MODE_FORCED,
    .filter = BME280_FILTER_OFF
};

int main(void)
{
    led_init();
    log_init();
    button_init();
    button_enable_interrupt();

    if (bme280_init(&sensor, BME280_I2C_ADDR) != BME280_OK)
    {
        log_print(LOG_ERROR, "sensor not found");
        led_set(true);

        while (1)
        {
        }
    }

    log_print(LOG_INFO, "sensor initialized");

    bme280_configure(&sensor, &sensor_config);

    while (1)
    {
        if (button_consume_press())
        {
            led_toggle();
        }

        bme280_reading_t reading;

        if (bme280_read(&sensor, &reading) == BME280_OK)
        {
            report_temperature(reading.temperature);
        }
        else
        {
            log_print(LOG_ERROR, "reading failed");
        }

        delay_seconds();
    }
}