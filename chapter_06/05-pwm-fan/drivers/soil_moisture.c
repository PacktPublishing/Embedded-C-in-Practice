#include "soil_moisture.h"

#include "adc.h"
#include "event_queue.h"

#define SOIL_ADC ADC_1
#define SOIL_CHANNEL 1u
#define SOIL_SAMPLE_PERIOD_MS 2000u
#define SOIL_IRQ_PRIORITY 5

void soil_moisture_init(void)
{
    adc_start_periodic(SOIL_ADC, SOIL_CHANNEL,
                       SOIL_SAMPLE_PERIOD_MS, SOIL_IRQ_PRIORITY);
}

void ADC_IRQHandler(void)
{
    uint16_t sample;

    if (adc_read_result(SOIL_ADC, &sample) == ADC_OK)
    {
        const event_t event =
        {
            .type = EVENT_SOIL_SAMPLE_READY,
            .data = sample
        };

        event_queue_put(&app_events, event);
    }
}
