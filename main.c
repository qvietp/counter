#include "stm32f10x.h"
#include "mpu6050.h"
#include "lcd_i2c.h"

void Delay_ms(uint32_t ms)
{
   volatile uint32_t i, j;
   for(i = 0 ; i < ms ; i++)
   {
      for(j = 0 ; j < 2000 ; j++)
      {
         __NOP(); 
      }
   }
}

int main(void)
{
    uint16_t step_count = 0;
    float acceleration[3];

    MPU6050_Init();
    LCD_I2C_Init();

    while (1)
    {
        MPU6050_GetAcceleration(acceleration);

        // Tính toán gia toc tang và kiem tra xem có the có mot buoc chân không
        float total_acceleration = acceleration[0] + acceleration[1] + acceleration[2];
        if (total_acceleration > 1.5) {
            step_count++;
            LCD_I2C_DisplayNumber(step_count);
        }

        Delay_ms(10); // Ðoi trong mot thoi gian ngan truoc khi doc du lieu moi tu MPU6050
    }
}
