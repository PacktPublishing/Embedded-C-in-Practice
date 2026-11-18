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
#include "stack_monitor.h"
#include "blink_code.h"
#include "gpio.h"
#include <stdio.h>

#define BME280_I2C_ADDR 0x76
#define BLINK_CODE_STACK 2u
#define DEBUG_PIN_PORT GPIO_B
#define DEBUG_PIN      4u
#define SMOKE_TICK_GUARD  100000u
#define SMOKE_PROBE_MAX   4095u

static bme280_t sensor;

static void debug_pin_init(void)
{
    static const gpio_config_t config =
    {
        .direction = GPIO_DIRECTION_OUTPUT,
        .output_type = GPIO_PUSH_PULL,
        .pull = GPIO_PULL_NONE
    };

    gpio_configure(DEBUG_PIN_PORT, DEBUG_PIN, &config);
}

static const bme280_config_t sensor_config =
{
    .temperature_oversampling = BME280_OVERSAMPLING_X1,
    .pressure_oversampling = BME280_OVERSAMPLING_X1,
    .humidity_oversampling = BME280_OVERSAMPLING_X1,
    .mode = BME280_MODE_FORCED,
    .filter = BME280_FILTER_OFF
};

static soft_timer_t report_timer = { .interval_ms = 1000u };

static volatile uint16_t last_probe_sample;
static volatile bool probe_sample_seen;

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

static void smoke_report(const char *name, bool passed)
{
    char message[32];

    snprintf(message, sizeof(message), "smoke %s: %s", name,
             passed ? "PASS" : "FAIL");

    log_info(message);
}

static bool smoke_test_sensor(void)
{
    bme280_reading_t reading;

    return bme280_read(&sensor, &reading) == BME280_OK;
}

static bool smoke_test_probe(void)
{
    return probe_sample_seen && (last_probe_sample <= SMOKE_PROBE_MAX);
}

static bool smoke_test_tick(void)
{
    uint32_t start = system_timer_get_ms();
    uint32_t guard = 0u;

    while ((system_timer_get_ms() == start) && (guard < SMOKE_TICK_GUARD))
    {
        guard++;
    }

    return system_timer_get_ms() != start;
}

static bool smoke_test_queue(void)
{
    event_queue_t queue = { 0 };
    const event_t sent = { .type = EVENT_UART_RX, .data = 0x5Au };
    event_t received;

    if (!event_queue_put(&queue, sent))
    {
        return false;
    }

    if (!event_queue_get(&queue, &received))
    {
        return false;
    }

    return (received.type == sent.type) && (received.data == sent.data);
}

static void smoke_run_all(void)
{
    log_info("smoke: start");
    smoke_report("sensor", smoke_test_sensor());
    smoke_report("probe", smoke_test_probe());
    smoke_report("tick", smoke_test_tick());
    smoke_report("queue", smoke_test_queue());
    log_info("smoke: done");
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
    else if (command == 'm')
    {
        report_stack_unused(stack_monitor_unused_bytes());
    }
    else if (command == 'x')
    {
        stack_monitor_force_overflow();
    }
    else if (command == 't')
    {
        smoke_run_all();
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

    log_debug("sensor: read start");

    if (bme280_read(&sensor, &reading) == BME280_OK)
    {
        log_debug("sensor: read ok");
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
    last_probe_sample = sample;
    probe_sample_seen = true;

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
    stack_monitor_init();
    debug_pin_init();
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
        log_error("sensor not found");
        led_set(true);

        while (1)
        {
        }
    }

    bme280_configure(&sensor, &sensor_config);

    log_info("ready: s start, e stop, 0-9 fan in alert, m stack, x test, t smoke, button stop");

    soft_timer_start(&report_timer, system_timer_get_ms());

    while (1)
    {
        event_t event;
        uint32_t now = system_timer_get_ms();

        gpio_write_pin(DEBUG_PIN_PORT, DEBUG_PIN, true);

        while (event_queue_get(&app_events, &event))
        {
            handle_event(event);
        }

        if (soft_timer_expired(&report_timer, now) &&
            (fsm_state() != STATE_IDLE))
        {
            read_temperature();
        }

        if (!stack_monitor_is_intact())
        {
            blink_code_halt(BLINK_CODE_STACK);
        }

        update_led(now);

        gpio_write_pin(DEBUG_PIN_PORT, DEBUG_PIN, false);
        power_manager_sleep();
    }
}
