#include "GPS.h"
#include "peripheral/uart_dev.h"
#include "peripheral/time_dev.h"

#include <math.h>
#include <stdlib.h>

void GPS_init(void)
{
    uart_dev_init(UART1, 12000000, 9600);
}

void GPS_read(void)
{

}


