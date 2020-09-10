#include "uart_dev.h"

/*--------------------LOW LEVEL UART HARDWARE IMPLEMENTATION, DEVICE SPECIFIC--------------------*/
/* EUSCI_A0
 * P1.2 -> RX
 * P1.3 -> TX
 *
 * EUSCI_A2
 * P3.2 -> RX
 * P3.3 -> TX
 *
 */

#ifdef MSP432P401R_RTOS_UART

#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"

bool uart_open[UART_COUNT] = {false, false};
UART_Handle UART_handles[UART_COUNT];

void uart_dev_init(uint32_t index, int fclock, int baudrate)
{
    if(uart_open[index]) return;

    UART_init();

    UART_Params UART_Config;

    UART_Params_init(&UART_Config);
    UART_Config.baudRate = baudrate;
    UART_Config.readMode = UART_MODE_BLOCKING;
    UART_Config.writeMode = UART_MODE_BLOCKING;
    UART_Config.readTimeout = UART_WAIT_FOREVER;
    UART_Config.writeTimeout = UART_WAIT_FOREVER;
    UART_Config.readEcho = UART_ECHO_OFF;
    UART_Config.dataLength = UART_LEN_8;

    uint32_t config_uart = (index == 0) ? CONFIG_UART_0 : CONFIG_UART_1;

    UART_handles[index] = UART_open(config_uart, &UART_Config);
    if (UART_handles[index] == NULL)
    {
        while(1);
    }

    uart_open[index] = true;
}

void uart_dev_print_char(uint32_t index, char c)
{
    UART_Handle *handle = &UART_handles[index];

    UART_write(*handle, (uint8_t*)&c, 1);
}

char uart_dev_get_char(uint32_t index)
{
    char c;
    UART_read(UART_handles[index], (uint8_t*)&c, 1);

    return c;
}

#elif defined(MSP432P401R_DRIVERLIB_UART)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

eUSCI_UART_ConfigV1 uart_config =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        13,                                      // BRDIV = 13
        0,                                       // UCxBRF = 0
        37,                                      // UCxBRS = 37
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_MSB_FIRST,                  // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};

void uart_dev_init(uint32_t index, int fclock, int baudrate)
{
    uint32_t N = fclock/baudrate;
    uint32_t UCBR = N/16;
    uint32_t UCBRF = (uint32_t)( ((float)(N/16.0) - (uint32_t)(N/16))*16 );

    //TODO: Find a better way to compute UCBRF and UCBRS values, THIS DOES NOT WORK!
    uart_config.clockPrescalar = UCBR;
    uart_config.firstModReg = (UCBRF << EUSCI_A_MCTLW_BRF_OFS);
    uart_config.secondModReg = 0;

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    UART_initModule(EUSCI_A0_BASE, &uart_config);
    UART_enableModule(EUSCI_A0_BASE);
}

void uart_dev_print_char(uint32_t index, char c)
{
    UART_transmitData(EUSCI_A0_BASE, (uint8_t)c);
}

char uart_dev_get_char(uint32_t index)
{
    return UART_receiveData(EUSCI_A0_BASE);
}

#elif defined(MSP432P401R_DRA_UART)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/inc/msp.h>

void uart_dev_init(uint32_t index, int fclock, int baudrate)
{
    P1->SEL0 |= (BIT2|BIT3);
    P1->SEL1 &= ~(BIT2|BIT3);

    uint32_t N = fclock/baudrate;
    uint32_t UCBR = N/16;
    uint32_t UCBRF = (uint32_t)( ((float)(N/16.0) - (uint32_t)(N/16))*16 );

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;
    EUSCI_A0->BRW = UCBR;
    EUSCI_A0->MCTLW = (UCBRF << EUSCI_A_MCTLW_BRF_OFS)| EUSCI_A_MCTLW_OS16;
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
}

void uart_dev_print_char(uint32_t index, char c)
{
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = c;
}

char uart_dev_get_char(uint32_t index)
{
    char c;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG));
    c = EUSCI_A0->RXBUF;

    return c;
}

#else
#warning USING UN-IMPLEMENTED UART COMMUNICATION, YOU MUST PROVIDE YOUR OWN SPECIFIC UART INTERFACE

void uart_dev_init(uint32_t index, int fclock, int baudrate)
{

}

void uart_dev_print_char(uint32_t index, char c)
{

}

char uart_dev_get_char(uint32_t index)
{

}

#endif




