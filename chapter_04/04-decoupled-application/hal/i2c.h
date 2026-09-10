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
    uint32_t pclk_hz;
    uint32_t bus_speed_hz;
} i2c_config_t;

typedef struct
{
    i2c_instance_t instance;
    i2c_config_t config;
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

i2c_status_t i2c_init(i2c_handle_t *handle,
                      i2c_instance_t instance,
                      const i2c_config_t *config);

i2c_status_t i2c_write_reg(i2c_handle_t *handle,
                           uint8_t dev_addr,
                           uint8_t reg_addr,
                           uint8_t value);

i2c_status_t i2c_read_regs(i2c_handle_t *handle,
                           uint8_t dev_addr,
                           uint8_t reg_addr,
                           uint8_t *buf,
                           uint32_t len);

i2c_status_t i2c_deinit(i2c_handle_t *handle);

#endif /* HAL_I2C_H */