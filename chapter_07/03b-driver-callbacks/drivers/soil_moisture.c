#include <stddef.h>

#include "soil_moisture.h"

#include "adc.h"

#define SOIL_ADC ADC_1
#define SOIL_CHANNEL 1u
#define SOIL_SAMPLE_PERIOD_MS 2000u
#define SOIL_IRQ_PRIORITY 5

static soil_moisture_callback_t sample_callback = NULL;

void soil_moisture_set_sample_callback(soil_moisture_callback_t callback)
{
    sample_callback = callback;
}

void soil_moisture_init(void)
{
    adc_start_periodic(SOIL_ADC, SOIL_CHANNEL,
                       SOIL_SAMPLE_PERIOD_MS, SOIL_IRQ_PRIORITY);
}

void ADC_IRQHandler(void)
{
    uint16_t sample;

    if ((adc_read_result(SOIL_ADC, &sample) == ADC_OK) &&
        (sample_callback != NULL))
    {
        sample_callback(sample);
    }
}
