#ifndef PERIPHERAL_SPI_DEV_H_
#define PERIPHERAL_SPI_DEV_H_

#include <stdint.h>

#define MSP432P401R_RTOS_SPI
//#define MSP432P401R_DRA_SPI
//#define MSP432P401R_DRIVERLIB_SPI

void spi_dev_init(uint32_t fclock, uint32_t fspi);
void spi_dev_write(uint8_t *data, uint32_t size);
void spi_dev_read(uint8_t *data, uint32_t size);
void spi_dev_cs(uint8_t state);

#endif /* PERIPHERAL_SPI_DEV_H_ */
