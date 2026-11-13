#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "led.h"
#include "log.h"
#include "system_timer.h"
#include "soft_timer.h"

static soft_timer_t status_timer = { .interval_ms = 100u };

#define STATUS_LINE_SIZE 64u

static char *format_status(uint32_t now)
{
    char *line = malloc(STATUS_LINE_SIZE);

    if (line != NULL)
    {
        snprintf(line, STATUS_LINE_SIZE,
                 "uptime %lu ms, line at 0x%08lx",
                 (unsigned long)now, (unsigned long)(uintptr_t)line);
    }

    return line;
}

/*
 * format_status() returns a line allocated from the heap. The caller
 * owns it, but nothing in the name says so, and this function never
 * frees it.
 */
static void report_status(uint32_t now)
{
    char *line = format_status(now);

    if (line != NULL)
    {
        log_print(LOG_INFO, line);
    }
}

int main(void)
{
    led_init();
    log_init();
    system_timer_init();

    soft_timer_start(&status_timer, system_timer_get_ms());

    while (1)
    {
        uint32_t now = system_timer_get_ms();

        if (soft_timer_expired(&status_timer, now))
        {
            report_status(now);
            led_toggle();
        }
    }
}
