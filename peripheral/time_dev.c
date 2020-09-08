#include "time_dev.h"

//TODO: Implement millis

#ifdef MSP432P401R_RTOS_TIME

#include <unistd.h>

void delay(int ms)
{
    usleep(1000*ms);
}

uint32_t millis(void)
{

}

#elif defined(MSP432P401R_DRIVERLIB_TIME)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void delay(int ms)
{
    while(ms--)
    {
        __delay_cycles(1200);
    }

}

uint32_t millis(void)
{

}

#elif defined(MSP432P401R_DRA_TIME)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/inc/msp.h>

void delay(int ms)
{
    while(ms--)
    {
        __delay_cycles(1200);
    }

}

uint32_t millis(void)
{

}

#endif





