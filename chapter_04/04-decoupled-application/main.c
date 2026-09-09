#include "led.h"
#include "button.h"
#include "bme280.h"
#include "debug.h"
#include "report_temperature.h"
#include "delay.h"

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
    debug_init();

    if (bme280_init(&sensor, BME280_I2C_ADDR) != BME280_OK)
    {
        debug_print(DEBUG_ERROR, "sensor not found");
        led_set(true);

        while (1)
        {
        }
    }

    bme280_configure(&sensor, &sensor_config);

    while (1)
    {
        bme280_reading_t reading;

        if (bme280_read(&sensor, &reading) == BME280_OK)
        {
            report_temperature(reading.temperature);
        }

        delay_seconds();
    }
}
