#include "MPU9250.h"
#include "peripheral/i2c_dev.h"
#include "peripheral/time_dev.h"

#include <math.h>


//Based on Kriswner and Betaflight

/*CALIBRATION DATA*/
static float mag_cal[3];

/*BIAS DATA*/
static int32_t accel_bias[3];
static int32_t gyro_bias[3];
static float mag_bias[3];

/*RESOLUTION DATA*/
uint8_t accel_res;
uint8_t gyro_res;
uint8_t mag_res;

/*SCALE DATA*/
static float mag_scale;
static float mag_scales[3];
static float accel_sca;
static float gyro_sca;

void MPU9250_init(void)
{
    i2c_dev_init(12000000, I2C_DEV_400KHZ);
    MPU9250_write_byte(PWR_MGMT_1, 0x00);
    delay(100);

    //Enable AK8963 magnetometer
    MPU9250_write_byte(INT_PIN_CFG, 0x02);

    //Reset value is +-250dps
    gyro_sca = 250.0f/32678.0f;
    gyro_res = GFS_250DPS;

    //Reset value is +-2g
    accel_sca = 2.0f/32768.0f;
    accel_res = AFS_2G;

    AK8963_init();
}

void AK8963_init(void)
{
    AK8963_write_byte(AK8963_CNTL, 0x00);
    delay(10);
    AK8963_write_byte(AK8963_CNTL, 0x0F);
    delay(10);

    uint8_t raw_data[3];
    i2c_dev_read(AK8963_ADDRESS, AK8963_ASAX, raw_data, 3);
    mag_cal[0] =(float)(raw_data[0] - 128.0f)/256.0f + 1.0f;
    mag_cal[1] =(float)(raw_data[1] - 128.0f)/256.0f + 1.0f;
    mag_cal[2] =(float)(raw_data[2] - 128.0f)/256.0f + 1.0f;

    AK8963_write_byte(AK8963_CNTL, 0x00);
    delay(10);
    AK8963_write_byte(AK8963_CNTL, MFS_16BITS << 4 | 0x02);
    delay(10);

    mag_res = MFS_16BITS;
    mag_scale = 10.0f*4912.0f/32760.f;
}

uint8_t MPU9250_who(void)
{
    uint8_t c;
    i2c_dev_read(MPU9250_1_ADDRESS, WHO_AM_I_MPU9250, &c, 1);

    return c;
}

uint8_t AK8963_who(void)
{
    uint8_t c;
    i2c_dev_read(AK8963_ADDRESS, WHO_AM_I_AK8963, &c, 1);

    return c;
}

void MPU9250_reset(void)
{
    MPU9250_write_byte(PWR_MGMT_1, 0x80);
    delay(100);
}

void MPU9250_raw_accelerometer(int16_t *accel_data)
{
    uint8_t raw_data[6];
    i2c_dev_read(MPU9250_1_ADDRESS, ACCEL_XOUT_H, raw_data, 6);

    accel_data[0] = (((int16_t)raw_data[0]) << 8) | raw_data[1];
    accel_data[1] = (((int16_t)raw_data[2]) << 8) | raw_data[3];
    accel_data[2] = (((int16_t)raw_data[4]) << 8) | raw_data[5];
}

void MPU9250_raw_gyroscope(int16_t *gyro_data)
{
    uint8_t raw_data[6];
    i2c_dev_read(MPU9250_1_ADDRESS, GYRO_XOUT_H, raw_data, 6);

    gyro_data[0] = (((int16_t)raw_data[0]) << 8) | raw_data[1];
    gyro_data[1] = (((int16_t)raw_data[2]) << 8) | raw_data[3];
    gyro_data[2] = (((int16_t)raw_data[4]) << 8) | raw_data[5];
}

void AK8963_raw_magnetometer(int16_t *mag_data)
{
    uint8_t raw_data[7];

    i2c_dev_read(AK8963_ADDRESS, AK8963_XOUT_L, raw_data, 7);
    if(!(raw_data[6] & 0x08))
    {
        mag_data[0] = (((int16_t)raw_data[1]) << 8) | raw_data[0];
        mag_data[1] = (((int16_t)raw_data[3]) << 8) | raw_data[2];
        mag_data[2] = (((int16_t)raw_data[5]) << 8) | raw_data[4];
    }
}

void MPU9250_accelerometer(float *accel)
{
    int16_t raw_data[3];
    MPU9250_raw_accelerometer(raw_data);

    raw_data[0] -= accel_bias[0];
    raw_data[1] -= accel_bias[1];
    raw_data[2] -= accel_bias[2];

    accel[0] = ((float)raw_data[0])*accel_sca;
    accel[1] = ((float)raw_data[1])*accel_sca;
    accel[2] = ((float)raw_data[2])*accel_sca;
}

void MPU9250_gyroscope(float *gyro)
{
    int16_t raw_data[3];
    MPU9250_raw_gyroscope(raw_data);

    raw_data[0] -= gyro_bias[0];
    raw_data[1] -= gyro_bias[1];
    raw_data[2] -= gyro_bias[2];

    gyro[0] = ((float)raw_data[0]*gyro_sca);
    gyro[1] = ((float)raw_data[1]*gyro_sca);
    gyro[2] = ((float)raw_data[2]*gyro_sca);

    //Convert to rads/s
    gyro[0] *= M_PI/180.0f;
    gyro[1] *= M_PI/180.0f;
    gyro[2] *= M_PI/180.0f;
}

void AK8963_magnetometer(float *mag)
{
    int16_t raw_data[3];
    AK8963_raw_magnetometer(raw_data);

    mag[0] = (float)raw_data[0]*mag_scale*mag_cal[0] - mag_bias[0];
    mag[1] = (float)raw_data[1]*mag_scale*mag_cal[1] - mag_bias[1];
    mag[2] = (float)raw_data[2]*mag_scale*mag_cal[2] - mag_bias[2];

    mag[0] *= mag_scales[0];
    mag[1] *= mag_scales[1];
    mag[2] *= mag_scales[2];
}

void MPU9250_accelerometer_range(uint8_t range)
{
    switch(range)
    {
    case AFS_2G:
        MPU9250_write_byte(ACCEL_CONFIG, AFS_2G << 3);
        accel_sca = 2.0f/32768.0f;
        accel_res = AFS_2G;
        break;
    case AFS_4G:
        MPU9250_write_byte(ACCEL_CONFIG, AFS_4G << 3);
        accel_sca = 4.0f/32768.0f;
        accel_res = AFS_4G;
        break;
    case AFS_8G:
        MPU9250_write_byte(ACCEL_CONFIG, AFS_8G << 3);
        accel_sca = 8.0f/32768.0f;
        accel_res = AFS_8G;
        break;
    case AFS_16G:
        MPU9250_write_byte(ACCEL_CONFIG, AFS_16G << 3);
        accel_sca = 16.0f/32768.0f;
        accel_res = AFS_16G;
        break;
    }
}

void MPU9250_gyroscope_range(uint8_t range)
{
    switch(range)
    {
    case GFS_250DPS:
        MPU9250_write_byte(GYRO_CONFIG, GFS_250DPS << 3);
        gyro_sca = 250.0f/32768.0f;
        gyro_res = GFS_250DPS;
        break;
    case GFS_500DPS:
        MPU9250_write_byte(GYRO_CONFIG, GFS_500DPS << 3);
        gyro_sca = 500.0f/32768.0f;
        gyro_res = GFS_500DPS;
        break;
    case GFS_1000DPS:
        MPU9250_write_byte(GYRO_CONFIG, GFS_1000DPS << 3);
        gyro_sca = 1000.0f/32768.0f;
        gyro_res = GFS_1000DPS;
        break;
    case GFS_2000DPS:
        MPU9250_write_byte(GYRO_CONFIG, GFS_2000DPS << 3);
        gyro_sca = 2500.0f/32768.0f;
        gyro_res = GFS_2000DPS;
        break;
    }
}

void MPU9250_calibrate_gyroscope(void)
{
    int16_t raw_data[3];

    uint32_t i;
    for(i = 0; i < CALIBRATION_SAMPLES; i++)
    {
        MPU9250_raw_gyroscope(raw_data);

        gyro_bias[0] += raw_data[0];
        gyro_bias[1] += raw_data[1];
        gyro_bias[2] += raw_data[2];

        delay(20);
    }

    gyro_bias[0] /= CALIBRATION_SAMPLES;
    gyro_bias[1] /= CALIBRATION_SAMPLES;
    gyro_bias[2] /= CALIBRATION_SAMPLES;
}

void MPU9250_calibrate_accelerometer(void)
{
    int16_t raw_data[3];

    uint32_t i;
    for(i = 0; i < CALIBRATION_SAMPLES; i++)
    {
        MPU9250_raw_accelerometer(raw_data);

        accel_bias[0] += raw_data[0];
        accel_bias[1] += raw_data[1];
        accel_bias[2] += raw_data[2];

        delay(20);
    }

    accel_bias[0] /= CALIBRATION_SAMPLES;
    accel_bias[1] /= CALIBRATION_SAMPLES;
    accel_bias[2] /= CALIBRATION_SAMPLES;

    switch(accel_res)
    {
    case AFS_2G:
        accel_bias[2] -= 32768/2;
        break;
    case AFS_4G:
        accel_bias[2] -= 32768/4;
        break;
    case AFS_8G:
        accel_bias[2] -= 32768/8;
        break;
    case AFS_16G:
        accel_bias[2] -= 32768/16;
        break;
    }
}

void AK8963_calibrate_magnetometer(void)
{

    int16_t raw_data[3];
    int16_t mag_max[] = {-32767, -32767, -32767};
    int16_t mag_min[] = { 32767,  32767,  32767};

    uint32_t i;
    for(i = 0; i < CALIBRATION_SAMPLES; i++)
    {
        AK8963_raw_magnetometer(raw_data);

        uint32_t j;
        for(j = 0; j < 3; j++)
        {
            mag_max[j] = (raw_data[j] > mag_max[j]) ? raw_data[j] : mag_max[j];
            mag_min[j] = (raw_data[j] < mag_min[j]) ? raw_data[j] : mag_min[j];
        }

        delay(150);
    }

    //Iron hard corrections
    int32_t mgb[3];
    mgb[0] = (mag_max[0] + mag_min[0])/2;
    mgb[1] = (mag_max[1] + mag_min[1])/2;
    mgb[2] = (mag_max[2] + mag_min[2])/2;

    mag_bias[0] = (float)mgb[0]*mag_scale*mag_cal[0];
    mag_bias[1] = (float)mgb[1]*mag_scale*mag_cal[1];
    mag_bias[2] = (float)mgb[2]*mag_scale*mag_cal[2];

    //Soft iron corrections
    int32_t mgs[3];
    mgs[0] = (mag_max[0] - mag_min[0])/2;
    mgs[1] = (mag_max[1] - mag_min[1])/2;
    mgs[2] = (mag_max[2] - mag_min[2])/2;

    float avg_rad = (mgs[0] + mgs[1] + mgs[2])/3.0f;

    mgs[0] = (mgs[0] == 0) ? 1 : mgs[0];
    mgs[1] = (mgs[1] == 0) ? 1 : mgs[1];
    mgs[2] = (mgs[2] == 0) ? 1 : mgs[2];

    mag_scales[0] = avg_rad/(float)mgs[0];
    mag_scales[1] = avg_rad/(float)mgs[1];
    mag_scales[2] = avg_rad/(float)mgs[2];
}


void AK8963_write_byte(uint8_t reg, uint8_t value)
{
    uint8_t cmd[] = {reg, value};
    i2c_dev_write(AK8963_ADDRESS, cmd, 2);
}

void MPU9250_write_byte(uint8_t reg, uint8_t value)
{
    uint8_t cmd[] = {reg, value};
    i2c_dev_write(MPU9250_1_ADDRESS, cmd, 2);
}

