#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include "stm32f10x.h"

#define LCD_ADDR 0x3F

void LCD_I2C_Init(void);
void LCD_I2C_DisplayNumber(uint16_t number);

#endif /* LCD_I2C_H_ */
