#ifndef DRIVERS_BME280_H
#define DRIVERS_BME280_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef enum
{
    BME280_OK = 0,
    BME280_ERROR_INVALID_ARG,
    BME280_ERROR_NOT_INITIALIZED,
    BME280_ERROR_BUS,
    BME280_ERROR_WRONG_DEVICE
} bme280_status_t;

typedef enum
{
    BME280_OVERSAMPLING_SKIP = 0,
    BME280_OVERSAMPLING_X1,
    BME280_OVERSAMPLING_X2,
    BME280_OVERSAMPLING_X4,
    BME280_OVERSAMPLING_X8,
    BME280_OVERSAMPLING_X16
} bme280_oversampling_t;

typedef enum
{
    BME280_MODE_SLEEP = 0,
    BME280_MODE_FORCED = 1,
    BME280_MODE_NORMAL = 3
} bme280_mode_t;

typedef enum
{
    BME280_FILTER_OFF = 0,
    BME280_FILTER_X2,
    BME280_FILTER_X4,
    BME280_FILTER_X8,
    BME280_FILTER_X16
} bme280_filter_t;

typedef struct
{
    bme280_oversampling_t temperature_oversampling;
    bme280_oversampling_t pressure_oversampling;
    bme280_oversampling_t humidity_oversampling;
    bme280_mode_t mode;
    bme280_filter_t filter;
} bme280_config_t;

typedef struct
{
    float temperature;
    float pressure;
    float humidity;
} bme280_reading_t;

/* Calibration data read at init and used for compensation in bme280_read().
 * Kept here because the compensation formulas require it. */
typedef struct
{
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;
} bme280_calibration_t;

typedef struct
{
    i2c_instance_t i2c;
    uint8_t address;
    bme280_config_t config;
    bme280_calibration_t calibration;
    bool initialized;
} bme280_t;

bme280_status_t bme280_init(bme280_t *dev, uint8_t address);

bme280_status_t bme280_configure(bme280_t *dev,
                                 const bme280_config_t *config);

bme280_status_t bme280_read(bme280_t *dev, bme280_reading_t *reading);

#endif // DRIVERS_BME280_H
