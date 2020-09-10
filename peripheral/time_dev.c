#include "time_dev.h"

//TODO: Implement millis

#ifdef MSP432P401R_RTOS_TIME

#include <ti/drivers/Timer.h>
#include <unistd.h>

#include "ti_drivers_config.h"

Timer_Handle timer;
volatile uint32_t timer_millis = 0;

void timer_dev_callback(Timer_Handle handle);

void time_dev_init(uint32_t fclock)
{
    Timer_init();

    Timer_Params params;
    Timer_Params_init(&params);
    params.period = 1000;       //1ms delay
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timer_dev_callback;

    timer = Timer_open(CONFIG_TIMER_0, &params);
    if(timer == NULL)
    {
        //Failed to open timer
        while(1);
    }

    if(Timer_start(timer) == Timer_STATUS_ERROR)
    {
        //Failed to start timer
        while(1);
    }
}

void delay(uint32_t ms)
{
    uint32_t start = millis();

    while((millis()-start) < ms)
    {
        usleep(1000);
    }
}

uint32_t millis(void)
{
    return timer_millis;
}

void timer_dev_callback(Timer_Handle handle)
{
    timer_millis++;
}

#elif defined(MSP432P401R_DRIVERLIB_TIME)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

volatile uint32_t timer_millis = 0;

void time_dev_init(uint32_t fclock)
{
    Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_BASE, fclock/1000);

    /* Enabling interrupts */
    Interrupt_enableInterrupt(INT_PORT1);
    Interrupt_enableInterrupt(INT_T32_INT1);
    Interrupt_enableMaster();
}

void delay(int ms)
{
    uint32_t start = millis();

    while((millis()-start) < ms)
    {
        //yield();
    }
}

uint32_t millis(void)
{
    return timer_millis;
}

void T32_INT1_IRQHandler(void)
{
    timer_millis++;
}

#elif defined(MSP432P401R_DRA_TIME)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/inc/msp.h>

volatile uint32_t timer_millis = 0;

void time_dev_init(uint32_t fclock)
{
    TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_MODE;
    TIMER32_1->LOAD = fclock/1000;

    NVIC_EnableIRQ(T32_INT1_IRQn);

    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE | TIMER32_CONTROL_IE;
}

void delay(int ms)
{
    uint32_t start = millis();

    while((millis()-start) < ms)
    {
        //yield();
    }
}

uint32_t millis(void)
{
    return timer_millis;
}

void T32_INT1_IRQHandler(void)
{
    timer_millis++;
}

#endif





