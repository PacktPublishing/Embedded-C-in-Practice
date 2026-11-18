#ifndef SOIL_MOISTURE_H
#define SOIL_MOISTURE_H

#include <stdint.h>

typedef void (*soil_moisture_callback_t)(uint16_t sample);

void soil_moisture_set_sample_callback(soil_moisture_callback_t callback);

void soil_moisture_init(void);

#endif // SOIL_MOISTURE_H
