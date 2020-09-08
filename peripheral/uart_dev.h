#ifndef UART_DEV_H_
#define UART_DEV_H_

//#define MSP432P401R_RTOS_UART
//#define MSP432P401R_DRIVERLIB_UART
#define MSP432P401R_DRA_UART

void uart_dev_init(int fclock, int baudrate);
void print_char(char c);
char get_char(void);

#endif /* UART_DEV_H_ */
