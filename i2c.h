#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"

void I2C_Init1(void);
void I2C_Write(uint8_t addr, uint8_t* data, uint16_t size);
void I2C_Read(uint8_t addr, uint8_t* read_addr, uint8_t read_addr_size, uint8_t* buffer, uint16_t size);

#endif /* I2C_H_ */
