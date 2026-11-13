#include <stdint.h>
#include <stdio.h>

#include "led.h"
#include "log.h"
#include "system_timer.h"
#include "soft_timer.h"
#include "pool.h"

static soft_timer_t status_timer = { .interval_ms = 100u };

/*
 * Returns a line taken from the pool. The caller owns it and must give
 * it back with pool_release().
 */
static char *format_status(uint32_t now)
{
    char *line = pool_alloc();

    if (line != NULL)
    {
        snprintf(line, POOL_BLOCK_SIZE,
                 "uptime %lu ms, line at 0x%08lx",
                 (unsigned long)now, (unsigned long)(uintptr_t)line);
    }

    return line;
}

static void report_status(uint32_t now)
{
    char *line = format_status(now);

    if (line != NULL)
    {
        log_print(LOG_INFO, line);
        pool_release(line);
    }
    else
    {
        log_print(LOG_ERROR, "status: no free line in the pool");
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
