#include "i2c.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include <stddef.h>

#define I2C_TIMEOUT_GUARD 100000

typedef struct
{
    I2C_TypeDef *peripheral;
    i2c_config_t config;
    bool initialized;
} i2c_handle_t;

static i2c_handle_t i2c1_handle;
static i2c_handle_t i2c2_handle;
static i2c_handle_t i2c3_handle;

static i2c_handle_t *get_handle(i2c_instance_t instance)
{
    switch (instance)
    {
        case I2C_1:
            return &i2c1_handle;

        case I2C_2:
            return &i2c2_handle;

        case I2C_3:
            return &i2c3_handle;

        default:
            return NULL;
    }
}

static I2C_TypeDef *get_instance(i2c_instance_t instance)
{
    switch (instance)
    {
        case I2C_1:
            return I2C1;

        case I2C_2:
            return I2C2;

        case I2C_3:
            return I2C3;

        default:
            return NULL;
    }
}

static void configure_pins(i2c_instance_t instance)
{
    switch (instance)
    {
        case I2C_1:
            gpio_configure_alternate(GPIO_B, 8, 4, GPIO_OPEN_DRAIN);
            gpio_configure_alternate(GPIO_B, 9, 4, GPIO_OPEN_DRAIN);
            break;

        case I2C_2:
            gpio_configure_alternate(GPIO_B, 10, 4, GPIO_OPEN_DRAIN);
            gpio_configure_alternate(GPIO_B, 3, 9, GPIO_OPEN_DRAIN);
            break;

        case I2C_3:
            gpio_configure_alternate(GPIO_A, 8, 4, GPIO_OPEN_DRAIN);
            gpio_configure_alternate(GPIO_C, 9, 4, GPIO_OPEN_DRAIN);
            break;

        default:
            break;
    }
}

static void i2c_enable_clock(i2c_instance_t instance)
{
    switch (instance)
    {
        case I2C_1:
            RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
            break;

        case I2C_2:
            RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
            break;

        case I2C_3:
            RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
            break;

        default:
            break;
    }
}

i2c_status_t i2c_init(i2c_instance_t instance,
                      const i2c_config_t *config)
{
    if (config == NULL)
    {
        return I2C_ERROR_INVALID_ARG;
    }

    if ((config->pclk_hz == 0) || (config->bus_speed_hz == 0))
    {
        return I2C_ERROR_INVALID_ARG;
    }

    I2C_TypeDef *peripheral = get_instance(instance);
    i2c_handle_t *handle = get_handle(instance);

    if ((peripheral == NULL) || (handle == NULL))
    {
        return I2C_ERROR_INVALID_ARG;
    }

    if (handle->initialized)
    {
        return I2C_OK;
    }

    handle->peripheral = peripheral;
    handle->config = *config;

    configure_pins(instance);
    i2c_enable_clock(instance);

    peripheral->CR1 &= ~I2C_CR1_PE;

    uint32_t pclk_mhz = config->pclk_hz / 1000000U;

    peripheral->CR2 = pclk_mhz;
    peripheral->CCR = config->pclk_hz / (2U * config->bus_speed_hz);
    peripheral->TRISE = pclk_mhz + 1;

    peripheral->CR1 |= I2C_CR1_PE;
    peripheral->CR1 |= I2C_CR1_ACK;

    handle->initialized = true;

    return I2C_OK;
}

static i2c_status_t i2c_start(I2C_TypeDef *i2c, uint8_t dev_addr, bool read)
{
    uint32_t guard = 0;

    i2c->CR1 |= I2C_CR1_START;

    while ((i2c->SR1 & I2C_SR1_SB) == 0)
    {
        if (++guard > I2C_TIMEOUT_GUARD)
        {
            return I2C_ERROR_TIMEOUT;
        }
    }

    i2c->DR = (uint8_t)((dev_addr << 1) | (read ? 1 : 0));

    guard = 0;

    while ((i2c->SR1 & I2C_SR1_ADDR) == 0)
    {
        if ((i2c->SR1 & I2C_SR1_AF) != 0)
        {
            i2c->CR1 |= I2C_CR1_STOP;
            return I2C_ERROR_NACK;
        }

        if (++guard > I2C_TIMEOUT_GUARD)
        {
            i2c->CR1 |= I2C_CR1_STOP;
            return I2C_ERROR_TIMEOUT;
        }
    }

    (void)i2c->SR2;
    return I2C_OK;
}

static i2c_status_t i2c_wait_txe(I2C_TypeDef *i2c)
{
    uint32_t guard = 0;

    while ((i2c->SR1 & I2C_SR1_TXE) == 0)
    {
        if (++guard > I2C_TIMEOUT_GUARD)
        {
            return I2C_ERROR_TIMEOUT;
        }
    }

    return I2C_OK;
}

static i2c_status_t i2c_wait_btf(I2C_TypeDef *i2c)
{
    uint32_t guard = 0;

    while ((i2c->SR1 & I2C_SR1_BTF) == 0)
    {
        if (++guard > I2C_TIMEOUT_GUARD)
        {
            return I2C_ERROR_TIMEOUT;
        }
    }

    return I2C_OK;
}

i2c_status_t i2c_write_reg(i2c_instance_t instance,
                           uint8_t dev_addr,
                           uint8_t reg_addr,
                           uint8_t value)
{
    i2c_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

    I2C_TypeDef *peripheral = handle->peripheral;

    i2c_status_t status = i2c_start(peripheral, dev_addr, false);

    if (status != I2C_OK)
    {
        return status;
    }

    peripheral->DR = reg_addr;
    status = i2c_wait_txe(peripheral);

    if (status != I2C_OK)
    {
        return status;
    }

    peripheral->DR = value;
    status = i2c_wait_btf(peripheral);

    if (status != I2C_OK)
    {
        return status;
    }

    peripheral->CR1 |= I2C_CR1_STOP;

    return I2C_OK;
}

i2c_status_t i2c_read_regs(i2c_instance_t instance,
                           uint8_t dev_addr,
                           uint8_t reg_addr,
                           uint8_t *buf,
                           uint32_t len)
{
    if ((buf == NULL) || (len == 0))
    {
        return I2C_ERROR_INVALID_ARG;
    }

    i2c_handle_t *handle = get_handle(instance);

    if ((handle == NULL) || !handle->initialized)
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

    I2C_TypeDef *peripheral = handle->peripheral;

    i2c_status_t status = i2c_start(peripheral, dev_addr, false);

    if (status != I2C_OK)
    {
        return status;
    }

    peripheral->DR = reg_addr;
    status = i2c_wait_txe(peripheral);

    if (status != I2C_OK)
    {
        return status;
    }

    peripheral->CR1 |= I2C_CR1_ACK;
    status = i2c_start(peripheral, dev_addr, true);

    if (status != I2C_OK)
    {
        return status;
    }

    for (uint32_t i = 0; i < len; i++)
    {
        if (i == (len - 1))
        {
            peripheral->CR1 &= ~I2C_CR1_ACK;
            peripheral->CR1 |= I2C_CR1_STOP;
        }

        uint32_t guard = 0;

        while ((peripheral->SR1 & I2C_SR1_RXNE) == 0)
        {
            if (++guard > I2C_TIMEOUT_GUARD)
            {
                return I2C_ERROR_TIMEOUT;
            }
        }

        buf[i] = (uint8_t)peripheral->DR;
    }

    return I2C_OK;
}

i2c_status_t i2c_deinit(i2c_instance_t instance)
{
    i2c_handle_t *handle = get_handle(instance);

    if (handle == NULL)
    {
        return I2C_ERROR_INVALID_ARG;
    }

    if (handle->initialized)
    {
        handle->peripheral->CR1 &= ~I2C_CR1_PE;
    }

    handle->initialized = false;

    return I2C_OK;
}