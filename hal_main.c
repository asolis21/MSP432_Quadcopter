#include "MPU9250.h"
#include "GPS.h"
#include "BME280.h"
#include "UARTDEBUG.h"
#include "madgwick.h"

#include "peripheral/time_dev.h"

#include <unistd.h>
#include <math.h>
#include <servo.h>
#include <time.h>

#define SMCLK_FREQUENCY     12000000;

void *mainThread(void *arg0)
{
    time_dev_init(SMCLK_FREQUENCY);
    UARTDEBUG_init(SMCLK_FREQUENCY, 9600);
    MPU9250_init();
    BME280_init();
    GPS_init();

    Madgwick_init();

    MPU9250_accelerometer_range(AFS_16G);

    uint8_t id0 = MPU9250_who();
    uint8_t id1 = AK8963_who();
    uint8_t id2 = BME280_who();

    MPU9250_calibrate_accelerometer();
    MPU9250_calibrate_gyroscope();
    AK8963_calibrate_magnetometer();

    float accel[3];
    float gyro[3];
    float mag[3];

    float pitch, roll, yaw;
    float altitude;

    uint32_t start;
    float dt = 0.0;

    while(1)
    {
        start = millis();

        MPU9250_accelerometer(accel);
        MPU9250_gyroscope(gyro);
        AK8963_magnetometer(mag);
        altitude = BME280_altitude(1013.25);

        GPS_read();

        dt = (millis() - start)/1e3;

        usleep(100000);
    }
}


