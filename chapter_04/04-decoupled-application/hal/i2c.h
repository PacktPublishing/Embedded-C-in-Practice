#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    I2C_1,
    I2C_2,
    I2C_3
} i2c_instance_t;

typedef struct
{
    i2c_instance_t instance;
    uint32_t pclk_mhz;
    bool initialized;
} i2c_handle_t;

typedef enum
{
    I2C_OK = 0,
    I2C_ERROR_INVALID_ARG,
    I2C_ERROR_NOT_INITIALIZED,
    I2C_ERROR_NACK,
    I2C_ERROR_TIMEOUT
} i2c_status_t;

i2c_status_t i2c_init(i2c_handle_t *i2c, i2c_instance_t instance,
                      uint32_t pclk_mhz);

i2c_status_t i2c_write_reg(i2c_handle_t *i2c, uint8_t dev_addr,
                           uint8_t reg_addr, uint8_t value);

i2c_status_t i2c_read_regs(i2c_handle_t *i2c, uint8_t dev_addr,
                           uint8_t reg_addr, uint8_t *buf, uint32_t len);

#endif
