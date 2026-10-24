#include "report.h"
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

void report_soil_moisture(uint16_t raw)
{
    char message[32];

    snprintf(message, sizeof(message),
             "soil moisture (raw): %u",
             (unsigned int)raw);

    log_print(LOG_INFO, message);
}

void report_fan_speed(uint8_t percent)
{
    char message[32];

    snprintf(message, sizeof(message),
             "fan speed: %u %%", (unsigned int)percent);

    log_print(LOG_INFO, message);
}