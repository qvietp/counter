#include "lcd_i2c.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>

void LCD_I2C_Init(void)
{
    // Kh?i t?o màn hình LCD
    uint8_t init_seq[] = {
        0x00, 0x38, 0x00, 0x39, 0x00, 0x14, 0x00, 0x73, 0x00, 0x56,
        0x00, 0x6C, 0x00, 0x38, 0x00, 0x0C, 0x00, 0x01};
    I2C_Write(LCD_ADDR, init_seq, sizeof(init_seq));
}

void LCD_I2C_DisplayNumber(uint16_t number)
{
    // Hi?n th? s? lên màn hình LCD
    char buffer[16];
    sprintf(buffer, "Step Count: %d", number);

    uint8_t addr = 0x80;
    I2C_Write(LCD_ADDR, &addr, 1);
    I2C_Write(LCD_ADDR, (uint8_t*)buffer, strlen(buffer));
}
