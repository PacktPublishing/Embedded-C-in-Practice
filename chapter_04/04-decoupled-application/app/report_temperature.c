#include "report_temperature.h"
#include "debug.h"
#include <stdio.h>

void report_temperature(float temperature)
{
    char message[32];

    snprintf(message, sizeof(message), "temperature: %.2f C", temperature);
    debug_print(DEBUG_INFO, message);
}
