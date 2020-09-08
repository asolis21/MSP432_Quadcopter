#include "spi_dev.h"

/*
 * SPI implementation for MSP432P401R
 * P1.6 -> MOSI
 * P1.7 -> MISO
 * P1.5 -> CLK
 * P6.4 -> CS
 */

#ifdef MSP432P401R_FREERTOS_SPI

#include "ti_drivers_config.h"
#include <ti/drivers/SPI.h>

void spi_dev_init(uint32_t fclock, uint32_t fspi)
{

}

void spi_write(uint8_t *data, uint32_t size)
{

}

void spi_read(uint8_t *data, uint32_t size)
{

}

void spi_cs(uint8_t state)
{

}

#elif defined(MSP432P401R_DRIVERLIB_SPI)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void spi_dev_init(uint32_t fclock, uint32_t fspi)
{

}

void spi_dev_write(uint8_t *data, uint32_t size)
{

}

void spi_dev_read(uint8_t *data, uint32_t size)
{

}

void spi_dev_cs(uint8_t state)
{

}


#elif defined(MSP432P401R_DRA_SPI)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/inc/msp.h>

void spi_dev_init(uint32_t fclock, uint32_t fspi)
{
    P6->DIR |= BIT4
    P6->OUT &= ~BIT4|BIT5;

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;         //EUSCI in reset to modify registers
    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_MST  |        //SPI master
                        EUSCI_B_CTLW0_SYNC |        //Synchronous mode
                        EUSCI_B_CTLW0_CKPL |        //Clock polarity high
                        EUSCI_B_CTLW0_MSB);         //MSB first

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;   //Select SMCLK, which runs at a default 3MHz frequency
    EUSCI_B0->BRW = fclock/(fspi+1);                //f_bitclok = f_BRCLK/(UCBR+1) = 1MHz

    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);

}

void spi_dev_write(uint8_t *data, uint32_t size)
{
    uint32_t i;
    for(i = 0; i < size; i++)
    {
        while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));
        EUSCI_B0->TXBUF = data[i];
    }
}

void spi_dev_read(uint8_t *data, uint32_t size)
{
    uint32_t i;
    for(i = 0; i < size; i++)
    {
        while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));
        data[i] = EUSCI_B0->RXBUF;
    }
}

void spi_dev_cs(uint8_t state)
{
    P6->OUT |= (state == 1) ? BIT4 : ~BIT4;
}

