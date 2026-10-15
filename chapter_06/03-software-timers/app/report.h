#ifndef APP_REPORT_H
#define APP_REPORT_H

#include <stdbool.h>
#include <stdint.h>

void report_temperature(float temperature);
void report_status(bool sensor_active, uint32_t loop_passes);

#endif // APP_REPORT_H
