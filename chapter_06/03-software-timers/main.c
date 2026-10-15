#include "led.h"
#include "bme280.h"
#include "log.h"
#include "report.h"
#include "button.h"
#include "console.h"
#include "event_queue.h"
#include "system_timer.h"
#include "soft_timer.h"

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

#define STATUS_INTERVAL_MS 5000u

static soft_timer_t report_timer = { .interval_ms = 1000u };
static soft_timer_t status_timer = { .interval_ms = STATUS_INTERVAL_MS };

int main(void)
{
        led_init();
    log_init();
    button_init();
    button_enable_interrupt();
    console_init();
    system_timer_init();

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

    uint32_t start = system_timer_get_ms();
    uint32_t loop_passes = 0u;

    soft_timer_start(&report_timer, start);
    soft_timer_start(&status_timer, start);

    while (1)
    {
        event_t event;
        bme280_reading_t reading;
        uint32_t now = system_timer_get_ms();

        loop_passes++;

        while (event_queue_get(&app_events, &event))
        {
            handle_event(event);
        }

        if (soft_timer_expired(&report_timer, now) && sensor_active &&
            (bme280_read(&sensor, &reading) == BME280_OK))
        {
            report_temperature(reading.temperature);
        }

        if (soft_timer_expired(&status_timer, now))
        {
            report_status(sensor_active, loop_passes);
            loop_passes = 0u;
        }
    }
}
