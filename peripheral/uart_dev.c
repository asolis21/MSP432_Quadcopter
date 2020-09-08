#include <peripheral/uart_dev.h>

/*UART0 Backend UART, used to communicate with PC. Use it for simple communication, or debugging*/

#ifdef MSP432P401R_RTOS_UART

#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"

UART_Handle uart_console;

void uart_dev_init(int fclock, int baudrate)
{
    UART_init();

    UART_Params UART0_Config;

    UART_Params_init(&UART0_Config);
    UART0_Config.baudRate = baudrate;
    UART0_Config.readMode = UART_MODE_BLOCKING;
    UART0_Config.writeMode = UART_MODE_BLOCKING;
    UART0_Config.readTimeout = UART_WAIT_FOREVER;
    UART0_Config.writeTimeout = UART_WAIT_FOREVER;
    UART0_Config.dataLength = UART_LEN_8;

    uart_console = UART_open(CONFIG_UART_0, &UART0_Config);
    if (uart_console == NULL)
    {
        while(1);
    }
}

void print_char(char c)
{
    UART_write(uart_console, (uint8_t*)&c, 1);
}

char get_char(void)
{
    char c;
    UART_read(uart_console, (uint8_t*)&c, 1);

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

void uart_dev_init(int fclock, int baudrate)
{
    uint32_t N = fclock/baudrate;
    uint32_t UCBR = N/16;
    uint32_t UCBRF = (uint32_t)( ((float)(N/16.0) - (uint32_t)(N/16))*16 );

    //TODO: Find a better way to compute UCBRF and UCBRS values
    uart_config.clockPrescalar = UCBR;
    uart_config.firstModReg = UCBRF;
    uart_config.secondModReg = 0;

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    UART_initModule(EUSCI_A0_BASE, &uart_config);
    UART_enableModule(EUSCI_A0_BASE);
}

void print_char(char c)
{
    UART_transmitData(EUSCI_A0_BASE, (uint8_t)c);
}

char get_char(void)
{
    return UART_receiveData(EUSCI_A0_BASE);
}

#elif defined(MSP432P401R_DRA_UART)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/inc/msp.h>

void uart_dev_init(int fclock, int baudrate)
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

void print_char(char c)
{
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = c;
}

char get_char(void)
{
    char c;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG));
    c = EUSCI_A0->RXBUF;

    return c;
}

#endif




