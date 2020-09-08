#ifndef I2C_DEV_H_
#define I2C_DEV_H_

#include <stdint.h>
#include <stdbool.h>

#define MSP432P401R_FREERTOS_I2C
//#define MSP432P401R_DRIVERLIB_I2C
//#define MSP432P401R_DRA_I2C

void i2c_init(void);
bool i2c_write(uint8_t slave_address, uint8_t *data, uint32_t size);
bool i2c_read(uint8_t slave_address, uint8_t reg, uint8_t *data, uint32_t size);

//TODO: Move this to a different file!
void delay(int ms);

#endif /* I2C_DEV_H_ */
