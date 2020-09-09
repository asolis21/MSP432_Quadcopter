#ifndef UART_DEV_H_
#define UART_DEV_H_

#define MSP432P401R_RTOS_UART
//#define MSP432P401R_DRIVERLIB_UART
//#define MSP432P401R_DRA_UART

#include <stdint.h>

#define UART_COUNT  2

#define UART0   0
#define UART1   1

void uart_dev_init(uint32_t index, int fclock, int baudrate);
void uart_dev_print_char(uint32_t index, char c);
char uart_dev_get_char(uint32_t index);

#endif /* UART_DEV_H_ */
