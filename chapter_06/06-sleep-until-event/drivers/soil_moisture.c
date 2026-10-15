#include "soil_moisture.h"

#include "adc.h"
#include "event_queue.h"

#define SOIL_ADC ADC_1
#define SOIL_CHANNEL 1u
#define SOIL_SAMPLE_PERIOD_MS 2000u
#define SOIL_IRQ_PRIORITY 5

/* Written only by ADC_IRQHandler, read by the main loop. */
static volatile uint16_t last_sample = 0u;

void soil_moisture_init(void)
{
    adc_start_periodic(SOIL_ADC, SOIL_CHANNEL,
                       SOIL_SAMPLE_PERIOD_MS, SOIL_IRQ_PRIORITY);
}

uint16_t soil_moisture_get_raw(void)
{
    return last_sample;
}

/*
 * The sample is stored before the event is published, so the main loop
 * finds it in place when the event arrives.
 */
void ADC_IRQHandler(void)
{
    uint16_t sample;

    if (adc_read_result(SOIL_ADC, &sample) == ADC_OK)
    {
        const event_t event = { .type = EVENT_SOIL_SAMPLE_READY };

        last_sample = sample;
        event_queue_put(&app_events, event);
    }
}
