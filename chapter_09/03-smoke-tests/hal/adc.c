#include "adc.h"

#include <stdbool.h>
#include <stddef.h>

#include "gpio.h"
#include "stm32f4xx.h"
#include "timer.h"

/*
 * On the STM32F4, a regular conversion can be started only by a fixed
 * list of trigger signals. We use TIM3's trigger output, code 8 (1000)
 * in the EXTSEL field.
 */
#define ADC_TRIGGER_TIMER TIMER_3
#define TIM3_TRGO_CODE 0x8u

static bool get_channel_pin(uint8_t channel, gpio_port_t *port,
                            uint8_t *pin)
{
    if (channel <= 7u)
    {
        *port = GPIO_A;
        *pin = channel;
    }
    else if (channel <= 9u)
    {
        *port = GPIO_B;
        *pin = (uint8_t)(channel - 8u);
    }
    else if (channel <= 15u)
    {
        *port = GPIO_C;
        *pin = (uint8_t)(channel - 10u);
    }
    else
    {
        return false;
    }

    return true;
}

static void set_longest_sample_time(ADC_TypeDef *adc, uint8_t channel)
{
    if (channel <= 9u)
    {
        adc->SMPR2 |= (0x7u << (channel * 3u));
    }
    else
    {
        adc->SMPR1 |= (0x7u << ((channel - 10u) * 3u));
    }
}

adc_status_t adc_start_periodic(adc_instance_t instance,
                                uint8_t channel,
                                uint32_t period_ms,
                                uint8_t priority)
{
    gpio_port_t port;
    uint8_t pin;

    if ((instance != ADC_1) || !get_channel_pin(channel, &port, &pin) ||
        (period_ms < 2u) || (period_ms > 0x10000u))
    {
        return ADC_ERROR_INVALID_ARG;
    }

    ADC_TypeDef *adc = ADC1;

    gpio_configure_analog(port, pin);

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    (void)RCC->APB2ENR;

    set_longest_sample_time(adc, channel);

    const timer_config_t schedule =
    {
        .mode = TIMER_MODE_PERIODIC,
        .tick_hz = 1000u,
        .period_ticks = period_ms
    };

    adc->SQR1 &= ~ADC_SQR1_L;
    adc->SQR3 = channel;
    adc->CR1 |= ADC_CR1_EOCIE;
    adc->CR2 |= (TIM3_TRGO_CODE << ADC_CR2_EXTSEL_Pos);
    adc->CR2 |= ADC_CR2_EXTEN_0 | ADC_CR2_ADON;

    NVIC_SetPriority(ADC_IRQn, priority);
    NVIC_EnableIRQ(ADC_IRQn);

    timer_init(ADC_TRIGGER_TIMER, &schedule);
    timer_enable_trigger_output(ADC_TRIGGER_TIMER);
    timer_start(ADC_TRIGGER_TIMER);

    return ADC_OK;
}

adc_status_t adc_read_result(adc_instance_t instance,
                             uint16_t *value)
{
    if ((instance != ADC_1) || (value == NULL))
    {
        return ADC_ERROR_INVALID_ARG;
    }

    if ((ADC1->SR & ADC_SR_EOC) == 0u)
    {
        return ADC_NO_DATA;
    }

    *value = (uint16_t)ADC1->DR;

    return ADC_OK;
}
