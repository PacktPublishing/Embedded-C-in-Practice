#include "report_temperature.h"
#include "log.h"
#include <stdio.h>

void report_temperature(float temperature)
{
    char message[32];

    int whole = (int)temperature;
    int decimal = (int)((temperature - (float)whole) * 100.0f);

    snprintf(message, sizeof(message),
             "temperature: %d.%02d C",
             whole, decimal);

    log_print(LOG_INFO, message);
}
