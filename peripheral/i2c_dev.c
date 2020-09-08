#include <peripheral/i2c_dev.h>

/*--------------------LOW LEVEL I2C HARDWARE IMPLEMENTATION, DEVICE SPECIFIC--------------------*/
/*
 * P1.6 -> SDA
 * P1.7 -> SCL
 */
#ifdef MSP432P401R_FREERTOS_I2C

#include <unistd.h>
#include <ti/drivers/I2C.h>
#include "ti_drivers_config.h"

I2C_Handle MPU9250_handle;
bool i2c_initialized = false;


void i2c_init(void)
{
    if(i2c_initialized) return;
    I2C_init();

    I2C_Params params;
    I2C_Params_init(&params);
    params.bitRate = I2C_400kHz;

    MPU9250_handle = I2C_open(CONFIG_I2C_0, &params);

    if(MPU9250_handle == 0)
    {
        //Failed to open I2C
        while(1);
    }

    i2c_initialized = true;
}

bool i2c_write(uint8_t slave_address, uint8_t *data, uint32_t size)
{
    I2C_Transaction t = {0};

    t.slaveAddress = slave_address;
    t.writeBuf = data;
    t.writeCount = size;
    t.readBuf = NULL;
    t.readCount = 0;

    return I2C_transfer(MPU9250_handle, &t);
}

bool i2c_read(uint8_t slave_address, uint8_t reg, uint8_t *data, uint32_t size)
{
    I2C_Transaction t = {0};

    t.slaveAddress = slave_address;
    t.readBuf = data;
    t.readCount = size;
    t.writeBuf = &reg;
    t.writeCount = 1;

    return I2C_transfer(MPU9250_handle, &t);
}

//TODO: Move this to a different file!
void delay(int ms)
{
    usleep(1000*ms);
}

#elif defined(MSP432P401R_DRIVERLIB_I2C)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

bool i2c_initialized = false;

const eUSCI_I2C_MasterConfig i2c_config =
{
    EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
    12000000,                               // SMCLK = 12MHz
    EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 100khz
    0,                                      // No byte counter threshold
    EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

void i2c_init(void)
{
    if(i2c_initialized) return;

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    I2C_initMaster(EUSCI_B0_BASE, &i2c_config);
    I2C_enableModule(EUSCI_B0_BASE);

    i2c_initialized = true;
}

bool i2c_write(uint8_t slave_adddress, uint8_t *data, uint32_t size)
{
    while (I2C_masterIsStopSent(EUSCI_B0_BASE));
    I2C_setSlaveAddress(EUSCI_B0_BASE, slave_adddress);

    I2C_masterSendMultiByteStart(EUSCI_B0_BASE, data[0]);

    uint32_t i;
    for(i = 1; i < size - 1; i++)
    {
        I2C_masterSendMultiByteNext(EUSCI_B0_BASE, data[i]);
    }

    I2C_masterSendMultiByteFinish(EUSCI_B0_BASE, data[i]);

    return true;
}

bool i2c_read(uint8_t slave_adddress, uint8_t reg, uint8_t *data, uint32_t size)
{
    I2C_setSlaveAddress(EUSCI_B0_BASE, slave_adddress);
    I2C_masterSendMultiByteStart(EUSCI_B0_BASE, reg);
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));

    I2C_masterReceiveStart(EUSCI_B0_BASE);

    while(I2C_masterIsStartSent(EUSCI_B0_BASE));

    uint32_t i;
    for(i = 0; i < size - 1; i++)
    {
        data[i] = I2C_masterReceiveSingle(EUSCI_B0_BASE);
    }

    data[i] = I2C_masterReceiveMultiByteFinish(EUSCI_B0_BASE);

    return true;
}

void delay(int ms)
{
    while(ms--)
    {
        __delay_cycles(1200);
    }

}

#elif defined(MSP432P401R_DRA_I2C)

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/inc/msp.h>

bool i2c_initialized = false;

void i2c_init(void)
{
    if(i2c_initialized) return;

    P1->SEL0 |= (BIT6|BIT7);
    P1->SEL1 &= ~(BIT6|BIT7);

    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_MODE_3 |       //I2C mode
                       EUSCI_B_CTLW0_MST    |       //Master mode
                       EUSCI_B_CTLW0_SYNC   |       //Sync mode
                       EUSCI_B_CTLW0_SSEL__SMCLK;   //SMCLK

    //EUSCI_B0->CTLW1 |= EUSCI_B_CTLW1_ASTP_2;        //Automatic stop generated
    EUSCI_B0->BRW = 30;                             //SMCLK/30 = 100KHz

    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    i2c_initialized = true;
}

 /*| S | Slave Address | R/W | ACK | Data | ACK | S | Slave Address | ACK Data | ACK | P | */
bool i2c_write(uint8_t slave_address, uint8_t *data, uint32_t size)
{
    /* Making sure the last transaction has been completely sent out */
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);

    EUSCI_B0->I2CSA = slave_address;

    //Sent start condition
    EUSCI_B0->CTLW0 |=  EUSCI_B_CTLW0_TR |   //I2C transmit
                        EUSCI_B_CTLW0_TXSTT; //Start condition

    //Poll for transmit interrupt flag and start condition flag.
    while((EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT) || !(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));

    //Send first byte
    EUSCI_B0->TXBUF = data[0];

    uint32_t i;
    for(i = 1; i < size; i++)
    {
        //Poll for TX flag
        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
        EUSCI_B0->TXBUF = data[i];
    }

    //Poll for last byte sent
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));

    //Send stop condition
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

    return true;
}

bool i2c_read(uint8_t slave_address, uint8_t reg,  uint8_t *data, uint32_t size)
{
    EUSCI_B0->I2CSA = slave_address;

    /*SEND REGISTER BYTE*/
    //Sent start condition
    EUSCI_B0->CTLW0 |=  EUSCI_B_CTLW0_TR |   //I2C transmit
                        EUSCI_B_CTLW0_TXSTT; //Start condition

    //Poll for transmit interrupt flag and start condition flag
    while((EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT) || !(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    //Send register value
    EUSCI_B0->TXBUF = reg;
    //Poll for transmission
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));

    /*START RECEIVE DATA*/
    EUSCI_B0->CTLW0 &=  ~EUSCI_B_CTLW0_TR;  //I2C receive
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT; //Start condition

    //Poll for start condition flag
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);

    uint32_t i;
    for(i = 0; i < size - 1; i++)
    {
        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0));
        data[i] = EUSCI_B0->RXBUF & EUSCI_B_RXBUF_RXBUF_MASK;
    }

    //Send stop condition
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
    //Poll stop condition
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
    //Poll RX flag
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0));
    //Read last data byte
    data[i] = EUSCI_B0->RXBUF & EUSCI_B_RXBUF_RXBUF_MASK;

    return true;
}

//TODO: Move this to a different file!
void delay(int ms)
{
    while(ms--)
    {
        __delay_cycles(1200);
    }

}

#else
#warning USING UN-IMPLEMENTED I2C COMMUNICATION, YOU MUST PROVIDE YOUR OWN SPECIFIC I2C INTERFACE
void i2c_init(void)
{

}

void i2c_write(uint8_t slave_adddress, uint8_t *data, uint32_t size)
{

}

void i2c_read(uint8_t slave_adddress, uint8_t *data, uint32_t size)
{

}

#endif




