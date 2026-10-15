#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef enum
{
    ADC_1
} adc_instance_t;

typedef enum
{
    ADC_OK = 0,
    ADC_ERROR_INVALID_ARG,
    ADC_NO_DATA
} adc_status_t;

adc_status_t adc_start_periodic(adc_instance_t instance,
                                uint8_t channel,
                                uint32_t period_ms,
                                uint8_t priority);

adc_status_t adc_read_result(adc_instance_t instance,
                             uint16_t *value);

#endif
