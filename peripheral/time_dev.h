#ifndef PERIPHERAL_TIME_DEV_H_
#define PERIPHERAL_TIME_DEV_H_

#include <stdint.h>

#define MSP432P401R_RTOS_TIME
//#define MSP432P401R_DRIVERLIB_TIME
//#define MSP432P401R_DRA_TIME

void time_dev_init(uint32_t fclock);
void delay(uint32_t ms);
uint32_t millis(void);

#endif /* PERIPHERAL_TIME_DEV_H_ */
