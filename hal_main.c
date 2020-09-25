#include "MPU6050.h"
#include "QMC5883.h"
#include "UARTDEBUG.h"

#include "EasyHal/time_dev.h"

#include <math.h>

void *mainThread(void *arg0)
{
    time_dev_init();
    UARTDEBUG_init(9600);
    MPU6050_init(MPU6050_DEFAULT_ADDRESS);
    QMC5883_init();

    uint8_t id = MPU6050_who();
    int16_t raw_accel[3];
    int16_t raw_gyro[3];
    int16_t raw_mag[3];

    float accel[3];
    float gyro[3];
    float mag[3];

    float a_pitch, a_roll;
    float g_pitch, g_roll, g_yaw;
    float m_yaw;

    uint32_t start;
    float dt = 0.0;

    g_pitch = 0.0;
    g_roll = 0.0;
    g_yaw = 0.0;

    uint32_t i;
    float gyro_bias[3];

    for(i = 0; i < 200; i++)
    {
        MPU6050_raw_gyroscope(raw_gyro);

        gyro_bias[0] += (float)raw_gyro[0]/151.0f;
        gyro_bias[1] += (float)raw_gyro[1]/151.0f;
        gyro_bias[2] += (float)raw_gyro[2]/151.0f;
    }

    gyro_bias[0] /= 200;
    gyro_bias[1] /= 200;
    gyro_bias[2] /= 200;

    while(1)
    {
        start = millis();

        MPU6050_raw_accelerometer(raw_accel);
        MPU6050_raw_gyroscope(raw_gyro);
        QMC5883_raw_magnetometer(raw_mag);

        accel[0] = (float)raw_accel[0]/16384.0f;
        accel[1] = (float)raw_accel[1]/16384.0f;
        accel[2] = (float)raw_accel[2]/16384.0f;

        gyro[0] = ((float)raw_gyro[0]/151.0f) - gyro_bias[0];
        gyro[1] = ((float)raw_gyro[1]/151.0f) - gyro_bias[1];
        gyro[2] = ((float)raw_gyro[2]/151.0f) - gyro_bias[2];

        mag[0] = (float)raw_mag[0]/3000.0f;
        mag[1] = (float)raw_mag[1]/3000.0f;
        mag[2] = (float)raw_mag[2]/3000.0f;

        //Compute roll and pitch angle from accelerometer
        a_roll = atan2f(accel[0], sqrt(accel[1]*accel[1] + accel[2]*accel[2])) * (180.0f/M_PI);
        a_pitch =  atan2f(accel[1], sqrt(accel[0]*accel[0] + accel[2]*accel[2])) * (180.0f/M_PI);

        //Integrate values from gyroscope to obtain pitch, roll and yaw
        g_pitch += gyro[0]*dt;
        g_roll += gyro[1]*dt;
        g_yaw += gyro[2]*dt;

        //Compute yaw angle from magnetometer
        m_yaw = atan2f(mag[1], mag[0]) * (180.0f/M_PI);
        m_yaw = (m_yaw > 360.0f) ? (m_yaw - 360.0f) : m_yaw;
        m_yaw = (m_yaw < 0) ? (m_yaw + 360.0f) : m_yaw;

        UARTDEBUG_printf("a_pitch = %f, a_roll = %f, ", a_pitch, a_roll);
        UARTDEBUG_printf("g_pitch = %f, g_roll = %f, g_yaw = %f, ", g_pitch, g_roll, g_yaw);
        UARTDEBUG_printf("m_yaw = %f, dt = %f\r\n", m_yaw, dt);

        dt = (millis() - start)/1e3;
    }
}


