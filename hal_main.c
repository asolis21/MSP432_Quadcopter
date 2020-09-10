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

#define SMCLK_FREQUENCY     12000000

void *mainThread(void *arg0)
{
    time_dev_init(SMCLK_FREQUENCY);
    UARTDEBUG_init(SMCLK_FREQUENCY, 9600);
    MPU9250_init();
    BME280_init();
    GPS_init();

    uint8_t id0 = MPU9250_who();
    uint8_t id1 = AK8963_who();
    uint8_t id2 = BME280_who();

    float accel[3];
    float gyro[3];
    float mag[3];
    float altitude;
    char gps_data[128];

    uint32_t start;
    float dt = 0.0;

    while(1)
    {
        start = millis();

        //MPU9250_accelerometer(accel);
        //MPU9250_gyroscope(gyro);
        //AK8963_magnetometer(mag);
        //altitude = BME280_altitude(1013.25);

        GPS_read(gps_data, 128);

        //UARTDEBUG_printf("ax = %f, ay = %f, az = %f\r\n", accel[0], accel[1], accel[2]);
        //UARTDEBUG_printf("gx = %f, gy = %f, gz = %f\r\n", gyro[0], gyro[1], gyro[2]);
        //UARTDEBUG_printf("mx = %f, my = %f, mz = %f\r\n", mag[0], mag[1], mag[2]);
        //UARTDEBUG_printf("Altitude = %f, dt = %f\r\n", altitude, dt);

        dt = (millis() - start)/1e3;
        UARTDEBUG_printf("dt = %f, \x1B[33mGPS RCV:\x1B[32m %s", dt, gps_data);

        //usleep(1000000);

    }
}


