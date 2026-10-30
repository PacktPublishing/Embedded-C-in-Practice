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
#include "fsm.h"
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
        fsm_dispatch(INPUT_OVER_THRESHOLD, 0u);
    }
    else
    {
        fsm_dispatch(INPUT_WITHIN_LIMITS, 0u);
    }
}

static void handle_command(uint8_t command)
{
    if (command == 's')
    {
        fsm_dispatch(INPUT_START, 0u);
    }
    else if (command == 'e')
    {
        fsm_dispatch(INPUT_STOP, 0u);
    }
    else if ((command >= '0') && (command <= '9'))
    {
        fsm_dispatch(INPUT_FAN_SPEED, (command - '0') * 10u);
    }
}

static void handle_probe_sample(uint16_t sample)
{
    if (fsm_state() != STATE_IDLE)
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
        fsm_dispatch(INPUT_BUTTON, 0u);
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

/*
 * Callbacks registered with the drivers. They run inside the drivers'
 * interrupt handlers, at preemption priority 5, so they only publish an
 * event and return. The drivers no longer know about the queue.
 */
static void on_button_press(void)
{
    const event_t event = { .type = EVENT_BUTTON_PRESSED };

    event_queue_put(&app_events, event);
}

static void on_console_byte(uint8_t byte)
{
    const event_t event = { .type = EVENT_UART_RX, .data = byte };

    event_queue_put(&app_events, event);
}

static void on_probe_sample(uint16_t sample)
{
    const event_t event =
    {
        .type = EVENT_SOIL_SAMPLE_READY,
        .data = sample
    };

    event_queue_put(&app_events, event);
}

/* The LED is an output of the state: off in IDLE, a one-second period
 * in MONITOR, a 250 ms period in ALERT. */
static void update_led(uint32_t now)
{
    fsm_state_t current = fsm_state();

    if (current == STATE_MONITOR)
    {
        led_set((now % 1000u) < 500u);
    }
    else if (current == STATE_ALERT)
    {
        led_set((now % 250u) < 125u);
    }
    else
    {
        led_set(false);
    }
}

int main(void)
{
    led_init();
    log_init();
    system_timer_init();
    button_init();
    button_set_press_callback(on_button_press);
    button_enable_interrupt();
    console_set_rx_callback(on_console_byte);
    console_init();
    soil_moisture_set_sample_callback(on_probe_sample);
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
            (fsm_state() != STATE_IDLE))
        {
            read_temperature();
        }

        update_led(now);
        power_manager_sleep();
    }
}
