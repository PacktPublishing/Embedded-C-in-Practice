#include "led.h"
#include "bme280.h"
#include "log.h"
#include "report.h"
#include "button.h"
#include "console.h"
#include "event_queue.h"
#include "system_timer.h"
#include "soft_timer.h"
#include "soil_moisture.h"
#include "fan.h"
#include "power_manager.h"
#include "app_fsm.h"
#include "limits.h"

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

static soft_timer_t report_timer = { .interval_ms = 1000u };

/*
 * Both readings end here: the probe sample from the queue and the
 * temperature from the loop. The machine receives one combined input.
 */
static void dispatch_limits(void)
{
    if (limits_temperature_high() || limits_probe_high())
    {
        app_fsm_dispatch(INPUT_OVER_THRESHOLD, 0u);
    }
    else
    {
        app_fsm_dispatch(INPUT_WITHIN_LIMITS, 0u);
    }
}

static void handle_command(uint8_t command)
{
    if (command == 's')
    {
        app_fsm_dispatch(INPUT_START, 0u);
    }
    else if (command == 'e')
    {
        app_fsm_dispatch(INPUT_STOP, 0u);
    }
    else if ((command >= '0') && (command <= '9'))
    {
        app_fsm_dispatch(INPUT_FAN_SPEED, (command - '0') * 10u);
    }
}

static void handle_probe_sample(uint16_t sample)
{
    if (app_fsm_state() != STATE_IDLE)
    {
        report_soil_moisture(sample);
        limits_check_probe(sample);
        dispatch_limits();
    }
}

/* handle_event() no longer decides anything: it translates events
 * into the inputs of the state machine. */
static void handle_event(event_t event)
{
    if (event.type == EVENT_BUTTON_PRESSED)
    {
        app_fsm_dispatch(INPUT_BUTTON, 0u);
    }
    else if (event.type == EVENT_UART_RX)
    {
        handle_command((uint8_t)event.data);
    }
    else if (event.type == EVENT_SOIL_SAMPLE_READY)
    {
        handle_probe_sample((uint16_t)event.data);
    }
}

static void read_temperature(void)
{
    bme280_reading_t reading;

    if (bme280_read(&sensor, &reading) == BME280_OK)
    {
        report_temperature(reading.temperature);
        limits_check_temperature(reading.temperature);
        dispatch_limits();
    }
}

int main(void)
{
    led_init();
    log_init();
    system_timer_init();
    button_init();
    button_enable_interrupt();
    console_init();
    soil_moisture_init();
    fan_init();
    power_manager_init();

    if (bme280_init(&sensor, BME280_I2C_ADDR) != BME280_OK)
    {
        log_print(LOG_ERROR, "sensor not found");
        led_set(true);

        while (1)
        {
        }
    }

    bme280_configure(&sensor, &sensor_config);

    log_print(LOG_INFO, "ready: s start, e stop, 0-9 fan in alert, button stop");

    soft_timer_start(&report_timer, system_timer_get_ms());

    while (1)
    {
        event_t event;
        uint32_t now = system_timer_get_ms();

        while (event_queue_get(&app_events, &event))
        {
            handle_event(event);
        }

        if (soft_timer_expired(&report_timer, now) &&
            (app_fsm_state() != STATE_IDLE))
        {
            read_temperature();
        }

        power_manager_sleep();
    }
}
