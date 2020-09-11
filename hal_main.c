#include "MPU9250.h"
#include "GPS.h"
#include "UARTDEBUG.h"

#include "peripheral/time_dev.h"

#include <unistd.h>

#define SMCLK_FREQUENCY     12000000

void *mainThread(void *arg0)
{
    time_dev_init(SMCLK_FREQUENCY);
    UARTDEBUG_init(SMCLK_FREQUENCY, 9600);
    MPU9250_init();

    uint8_t id0 = MPU9250_who();
    uint8_t id1 = AK8963_who();

    int16_t accel[3];
    int16_t gyro[3];
    int16_t mag[3];

    uint32_t start;
    float dt = 0.0;

    while(1)
    {
        start = millis();

        MPU9250_raw_accelerometer(accel);
        MPU9250_raw_gyroscope(gyro);
        AK8963_raw_magnetometer(mag);

        UARTDEBUG_printf("ax = %i, ay = %i, az = %i, ", accel[0], accel[1], accel[2]);
        UARTDEBUG_printf("gx = %i, gy = %i, gz = %i, ", gyro[0], gyro[1], gyro[2]);
        UARTDEBUG_printf("mx = %i, my = %i, mz = %i, dt = %f\r\n", mag[0], mag[1], mag[2], dt);

        dt = (millis() - start)/1e3;
    }
}


