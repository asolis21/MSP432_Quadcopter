#ifndef PERIPHERAL_TIME_DEV_H_
#define PERIPHERAL_TIME_DEV_H_

#include <stdint.h>

#define MSP432P401R_RTOS_TIME
//#define MSP432P401R_DRA_TIME
//#define MSP432P401R_DRIVERLIB_TIME

void delay(int ms);
uint32_t millis(void);

#endif /* PERIPHERAL_TIME_DEV_H_ */
