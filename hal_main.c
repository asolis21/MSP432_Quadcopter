#include "MPU6050.h"
#include "GPS.h"
#include "UARTDEBUG.h"

#include "EasyHal/time_dev.h"

void *mainThread(void *arg0)
{
    time_dev_init();
    UARTDEBUG_init(9600);
    MPU6050_init(MPU6050_DEFAULT_ADDRESS);

    uint8_t id = MPU6050_who();
    int16_t raw_accel[3];
    int16_t raw_gyro[3];

    float accel[3];
    float gyro[3];

    uint32_t start;
    float dt = 0.0;

    while(1)
    {
        start = millis();

        MPU6050_raw_accelerometer(raw_accel);
        MPU6050_raw_gyroscope(raw_gyro);

        accel[0] = (float)raw_accel[0]/16384.0f;
        accel[1] = (float)raw_accel[1]/16384.0f;
        accel[2] = (float)raw_accel[2]/16384.0f;

        gyro[0] = (float)raw_gyro[0]/151.0f;
        gyro[1] = (float)raw_gyro[1]/151.0f;
        gyro[2] = (float)raw_gyro[2]/151.0f;

        UARTDEBUG_printf("ax = %f, ay = %f, az = %f, ", accel[0], accel[1], accel[2]);
        UARTDEBUG_printf("gx = %f, gy = %f, gz = %f, dt = %f\r\n", gyro[0], gyro[1], gyro[2], dt);

        dt = (millis() - start)/1e3;
    }
}


