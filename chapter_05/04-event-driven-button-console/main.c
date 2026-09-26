#include "led.h"
#include "bme280.h"
#include "log.h"
#include "report_temperature.h"
#include "delay.h"
#include "button.h"
#include "console.h"
#include "event_queue.h"

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

static bool sensor_active = false;

static void handle_event(event_t event)
{
    if (event.type == EVENT_BUTTON_PRESSED)
    {
        log_print(LOG_INFO, "button pressed event");
        led_toggle();
    }
    else if (event.type == EVENT_UART_RX)
    {
        if (event.data == 's')
        {
            log_print(LOG_INFO, "sensor started event");
            sensor_active = true;
        }
        else if (event.data == 'e')
        {
            log_print(LOG_INFO, "sensor stopped event");
            sensor_active = false;
        }
    }
}

int main(void)
{
    led_init();
    log_init();
    button_init();
    button_enable_interrupt();
    console_init();

    if (bme280_init(&sensor, BME280_I2C_ADDR) != BME280_OK)
    {
        log_print(LOG_ERROR, "sensor not found");
        led_set(true);

        while (1)
        {
        }
    }

    log_print(LOG_INFO, "sensor initialized");
    log_print(LOG_INFO, "type 's' to start, 'e' to stop");

    bme280_configure(&sensor, &sensor_config);

    while (1)
    {
        event_t event;
        bme280_reading_t reading;

        while (event_queue_get(&app_events, &event))
        {
            handle_event(event);
        }

        if (sensor_active)
        {
            if (bme280_read(&sensor, &reading) == BME280_OK)
            {
                report_temperature(reading.temperature);
            }
        }

        delay_seconds();
    }
}