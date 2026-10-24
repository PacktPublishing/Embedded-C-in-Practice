#ifndef APP_REPORT_H
#define APP_REPORT_H

#include <stdbool.h>
#include <stdint.h>

void report_temperature(float temperature);
void report_soil_moisture(uint16_t raw);
void report_fan_speed(uint8_t percent);

#endif // APP_REPORT_H
