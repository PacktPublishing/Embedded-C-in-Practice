#include "limits.h"

/*
 * Each reading has two limits: it must rise above the enter limit to
 * set its flag, and fall below the exit limit to clear it. The values
 * are meant for a test on the bench, with a potentiometer on the
 * probe pin and a warm finger on the ambient sensor.
 */
#define TEMP_ENTER_C    30.0f
#define TEMP_EXIT_C     28.0f
#define PROBE_ENTER_RAW 3000.0f
#define PROBE_EXIT_RAW  2800.0f

static bool temp_high = false;
static bool probe_high = false;

static bool check_limit(bool was_high, float value,
                        float enter, float exit)
{
    if (was_high)
    {
        return value > exit;
    }

    return value > enter;
}

void limits_reset(void)
{
    temp_high = false;
    probe_high = false;
}

void limits_check_temperature(float celsius)
{
    temp_high = check_limit(temp_high, celsius,
                            TEMP_ENTER_C, TEMP_EXIT_C);
}

void limits_check_probe(uint16_t raw)
{
    probe_high = check_limit(probe_high, (float)raw,
                             PROBE_ENTER_RAW, PROBE_EXIT_RAW);
}

bool limits_exceeded(void)
{
    return temp_high || probe_high;
}
