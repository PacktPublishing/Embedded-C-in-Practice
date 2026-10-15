#include "timer.h"

#include <stddef.h>

#include "clock.h"
#include "stm32f4xx.h"

/*
 * TIM2, TIM3, and TIM4 sit on APB1. Their clock equals the APB1 clock
 * as long as the APB1 prescaler is 1, its reset value. With any other
 * prescaler, the timer clock is twice the APB1 clock.
 */
#define TIMER_CLOCK_HZ APB1_CLOCK_HZ

typedef struct
{
    TIM_TypeDef *peripheral;
    bool initialized;
} timer_handle_t;

static timer_handle_t timer2_handle;
static timer_handle_t timer3_handle;
static timer_handle_t timer4_handle;

static TIM_TypeDef *get_instance(timer_instance_t instance)
{
    switch (instance)
    {
        case TIMER_2:
            return TIM2;

        case TIMER_3:
            return TIM3;

        case TIMER_4:
            return TIM4;

        default:
            return NULL;
    }
}

static timer_handle_t *get_handle(timer_instance_t instance)
{
    switch (instance)
    {
        case TIMER_2:
            return &timer2_handle;

        case TIMER_3:
            return &timer3_handle;

        case TIMER_4:
            return &timer4_handle;

        default:
            return NULL;
    }
}

static IRQn_Type get_irq(timer_instance_t instance)
{
    switch (instance)
    {
        case TIMER_2:
            return TIM2_IRQn;

        case TIMER_3:
            return TIM3_IRQn;

        default:
            return TIM4_IRQn;
    }
}

static uint32_t get_max_period(timer_instance_t instance)
{
    return (instance == TIMER_2) ? 0xFFFFFFFFu : 0x10000u;
}

static void timer_enable_clock(TIM_TypeDef *timer)
{
    if (timer == TIM2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    }
    else if (timer == TIM3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    }
    else
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    }

    (void)RCC->APB1ENR;
}

timer_status_t timer_init(timer_instance_t instance,
                          const timer_config_t *config)
{
    timer_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || (config == NULL))
    {
        return TIMER_ERROR_INVALID_ARG;
    }

    if (handle->initialized)
    {
        return TIMER_OK;
    }

    if ((config->tick_hz == 0u) || (config->tick_hz > TIMER_CLOCK_HZ) ||
        ((TIMER_CLOCK_HZ / config->tick_hz) > 0x10000u) ||
        (config->period_ticks < 2u) ||
        (config->period_ticks > get_max_period(instance)))
    {
        return TIMER_ERROR_INVALID_ARG;
    }

    TIM_TypeDef *timer = get_instance(instance);

    timer_enable_clock(timer);

    timer->PSC = (TIMER_CLOCK_HZ / config->tick_hz) - 1u;
    timer->ARR = config->period_ticks - 1u;

    if (config->mode == TIMER_MODE_ONE_SHOT)
    {
        timer->CR1 |= TIM_CR1_OPM;
    }

    timer->EGR = TIM_EGR_UG;
    timer->SR = ~TIM_SR_UIF;

    handle->peripheral = timer;
    handle->initialized = true;

    return TIMER_OK;
}

void timer_start(timer_instance_t instance)
{
    timer_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return;
    }

    handle->peripheral->CNT = 0u;
    handle->peripheral->CR1 |= TIM_CR1_CEN;
}

void timer_enable_update_interrupt(timer_instance_t instance,
                                   uint8_t priority)
{
    timer_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return;
    }

    handle->peripheral->DIER |= TIM_DIER_UIE;

    NVIC_SetPriority(get_irq(instance), priority);
    NVIC_EnableIRQ(get_irq(instance));
}

bool timer_is_update_pending(timer_instance_t instance)
{
    timer_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return false;
    }

    return (handle->peripheral->SR & TIM_SR_UIF) != 0u;
}

void timer_clear_update(timer_instance_t instance)
{
    timer_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return;
    }

    handle->peripheral->SR = ~TIM_SR_UIF;
}
