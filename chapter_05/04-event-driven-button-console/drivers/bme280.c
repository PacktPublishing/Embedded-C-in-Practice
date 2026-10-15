#include "bme280.h"
#include <string.h>

#define BME280_CHIP_ID 0x60u
#define BMP280_CHIP_ID 0x58u

#ifndef BME280_ACCEPT_BMP280
#define BME280_ACCEPT_BMP280 0
#endif

#define BME280_I2C_PCLK_MHZ  16

#define BME280_REG_ID         0xD0
#define BME280_REG_CALIB_1    0x88
#define BME280_REG_CALIB_1_LEN 26
#define BME280_REG_CALIB_2    0xE1
#define BME280_REG_CALIB_2_LEN 7
#define BME280_REG_CTRL_HUM  0xF2
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_CONFIG    0xF5
#define BME280_REG_DATA       0xF7
#define BME280_REG_DATA_LEN    8

static bme280_status_t bme280_write_config_registers(bme280_t *dev)
{
#if !BME280_ACCEPT_BMP280
    uint8_t ctrl_hum = (uint8_t)dev->config.humidity_oversampling;

    if (i2c_write_reg(dev->i2c, dev->address, BME280_REG_CTRL_HUM,
                      ctrl_hum) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }
#endif

    uint8_t config = (uint8_t)(dev->config.filter << 2);

    if (i2c_write_reg(dev->i2c, dev->address, BME280_REG_CONFIG,
                      config) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }

    uint8_t ctrl_meas = (uint8_t)((dev->config.temperature_oversampling << 5)
                                 | (dev->config.pressure_oversampling << 2)
                                 | dev->config.mode);

    if (i2c_write_reg(dev->i2c, dev->address, BME280_REG_CTRL_MEAS,
                      ctrl_meas) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }

    return BME280_OK;
}

static bme280_status_t bme280_read_calibration(bme280_t *dev)
{
    uint8_t buf1[BME280_REG_CALIB_1_LEN];
#if !BME280_ACCEPT_BMP280
    uint8_t buf2[BME280_REG_CALIB_2_LEN];
#endif

    if (i2c_read_regs(dev->i2c, dev->address, BME280_REG_CALIB_1,
                      buf1, BME280_REG_CALIB_1_LEN) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }

#if !BME280_ACCEPT_BMP280
    if (i2c_read_regs(dev->i2c, dev->address, BME280_REG_CALIB_2,
                      buf2, BME280_REG_CALIB_2_LEN) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }
#endif

    bme280_calibration_t *c = &dev->calibration;

    c->dig_T1 = (uint16_t)(buf1[0] | (buf1[1] << 8));
    c->dig_T2 = (int16_t)(buf1[2] | (buf1[3] << 8));
    c->dig_T3 = (int16_t)(buf1[4] | (buf1[5] << 8));

    c->dig_P1 = (uint16_t)(buf1[6] | (buf1[7] << 8));
    c->dig_P2 = (int16_t)(buf1[8] | (buf1[9] << 8));
    c->dig_P3 = (int16_t)(buf1[10] | (buf1[11] << 8));
    c->dig_P4 = (int16_t)(buf1[12] | (buf1[13] << 8));
    c->dig_P5 = (int16_t)(buf1[14] | (buf1[15] << 8));
    c->dig_P6 = (int16_t)(buf1[16] | (buf1[17] << 8));
    c->dig_P7 = (int16_t)(buf1[18] | (buf1[19] << 8));
    c->dig_P8 = (int16_t)(buf1[20] | (buf1[21] << 8));
    c->dig_P9 = (int16_t)(buf1[22] | (buf1[23] << 8));

    c->dig_H1 = buf1[25];

#if !BME280_ACCEPT_BMP280
    c->dig_H2 = (int16_t)(buf2[0] | (buf2[1] << 8));
    c->dig_H3 = buf2[2];
    c->dig_H4 = (int16_t)((buf2[3] << 4) | (buf2[4] & 0x0F));
    c->dig_H5 = (int16_t)((buf2[5] << 4) | (buf2[4] >> 4));
    c->dig_H6 = (int8_t)buf2[6];
#endif

    return BME280_OK;
}

bme280_status_t bme280_init(bme280_t *dev, uint8_t address)
{
    if (dev == NULL)
    {
        return BME280_ERROR_INVALID_ARG;
    }

    memset(dev, 0, sizeof(*dev));

    static const i2c_config_t i2c_config =
    {
        .bus_speed_hz = 100000
    };

    dev->i2c = I2C_1;
    dev->address = address;

    if (i2c_init(dev->i2c, &i2c_config) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }

    uint8_t chip_id = 0;

    if (i2c_read_regs(dev->i2c, address, BME280_REG_ID, &chip_id, 1) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }

    if ((chip_id != BME280_CHIP_ID) &&
        !(BME280_ACCEPT_BMP280 && (chip_id == BMP280_CHIP_ID)))
    {
        return BME280_ERROR_WRONG_DEVICE;
    }

    if (bme280_read_calibration(dev) != BME280_OK)
    {
        return BME280_ERROR_BUS;
    }

    dev->initialized = true;

    return BME280_OK;
}

bme280_status_t bme280_configure(bme280_t *dev,
                                 const bme280_config_t *config)
{
    if ((dev == NULL) || (config == NULL))
    {
        return BME280_ERROR_INVALID_ARG;
    }

    if (!dev->initialized)
    {
        return BME280_ERROR_NOT_INITIALIZED;
    }

    dev->config = *config;

    return bme280_write_config_registers(dev);
}

/* The Bosch integer compensation formulas, transcribed once from the
 * datasheet as the book describes and never touched again. t_fine is
 * shared between the temperature and the pressure/humidity formulas,
 * exactly as the datasheet specifies. */
static int32_t compensate_temperature(const bme280_calibration_t *c,
                                      int32_t adc_T, int32_t *t_fine)
{
    int32_t var1, var2, T;

    var1 = ((((adc_T >> 3) - ((int32_t)c->dig_T1 << 1))) * (int32_t)c->dig_T2) >> 11;
    var2 = (((((adc_T >> 4) - (int32_t)c->dig_T1) * ((adc_T >> 4) - (int32_t)c->dig_T1)) >> 12)
            * (int32_t)c->dig_T3) >> 14;

    *t_fine = var1 + var2;
    T = (*t_fine * 5 + 128) >> 8;

    return T;
}

static uint32_t compensate_pressure(const bme280_calibration_t *c,
                                    int32_t adc_P, int32_t t_fine)
{
    int64_t var1, var2, p;

    var1 = (int64_t)t_fine - 128000;
    var2 = var1 * var1 * (int64_t)c->dig_P6;
    var2 = var2 + ((var1 * (int64_t)c->dig_P5) << 17);
    var2 = var2 + (((int64_t)c->dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)c->dig_P3) >> 8)
         + ((var1 * (int64_t)c->dig_P2) << 12);
    var1 = ((((int64_t)1 << 47) + var1)) * (int64_t)c->dig_P1 >> 33;

    if (var1 == 0)
    {
        return 0;
    }

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = ((int64_t)c->dig_P9 * (p >> 13) * (p >> 13)) >> 25;
    var2 = ((int64_t)c->dig_P8 * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)c->dig_P7) << 4);

    return (uint32_t)p;
}

static uint32_t compensate_humidity(const bme280_calibration_t *c,
                                    int32_t adc_H, int32_t t_fine)
{
    int32_t v_x1;

    v_x1 = t_fine - 76800;
    v_x1 = (((((adc_H << 14) - ((int32_t)c->dig_H4 << 20) - ((int32_t)c->dig_H5 * v_x1))
             + 16384) >> 15)
            * ((((((v_x1 * (int32_t)c->dig_H6) >> 10)
                  * (((v_x1 * (int32_t)c->dig_H3) >> 11) + 32768)) >> 10)
                + 2097152) * (int32_t)c->dig_H2 + 8192) >> 14);
    v_x1 = v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)c->dig_H1) >> 4);
    v_x1 = (v_x1 < 0) ? 0 : v_x1;
    v_x1 = (v_x1 > 419430400) ? 419430400 : v_x1;

    return (uint32_t)(v_x1 >> 12);
}

bme280_status_t bme280_read(bme280_t *dev, bme280_reading_t *reading)
{
    if ((dev == NULL) || (reading == NULL))
    {
        return BME280_ERROR_INVALID_ARG;
    }

    if (!dev->initialized)
    {
        return BME280_ERROR_NOT_INITIALIZED;
    }

    if (dev->config.mode == BME280_MODE_FORCED)
    {
        uint8_t ctrl_meas = (uint8_t)((dev->config.temperature_oversampling << 5)
                                     | (dev->config.pressure_oversampling << 2)
                                     | BME280_MODE_FORCED);

        if (i2c_write_reg(dev->i2c, dev->address, BME280_REG_CTRL_MEAS,
                          ctrl_meas) != I2C_OK)
        {
            return BME280_ERROR_BUS;
        }
    }

    uint8_t raw[BME280_REG_DATA_LEN];

    if (i2c_read_regs(dev->i2c, dev->address, BME280_REG_DATA, raw,
                      BME280_REG_DATA_LEN) != I2C_OK)
    {
        return BME280_ERROR_BUS;
    }

    int32_t raw_pressure = ((int32_t)raw[0] << 12)
                         | ((int32_t)raw[1] << 4)
                         | ((int32_t)raw[2] >> 4);

    int32_t raw_temperature = ((int32_t)raw[3] << 12)
                            | ((int32_t)raw[4] << 4)
                            | ((int32_t)raw[5] >> 4);

#if !BME280_ACCEPT_BMP280
    int32_t raw_humidity = ((int32_t)raw[6] << 8)
                         | (int32_t)raw[7];
#endif
    int32_t t_fine = 0;
    int32_t temperature = compensate_temperature(&dev->calibration,
                                                  raw_temperature, &t_fine);
    uint32_t pressure = compensate_pressure(&dev->calibration, raw_pressure,
                                            t_fine);
#if !BME280_ACCEPT_BMP280
    uint32_t humidity = compensate_humidity(&dev->calibration, raw_humidity,
                                            t_fine);
#endif

    reading->temperature = (float)temperature / 100.0f;
    reading->pressure = (float)pressure / 256.0f;
#if !BME280_ACCEPT_BMP280
    reading->humidity = (float)humidity / 1024.0f;
#else
    reading->humidity = 0.0f;
#endif

    return BME280_OK;
}
