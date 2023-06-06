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
         __NOP(); //H�m assembler d?ng vi?c x? l� c?a CPU trong m?t chu k? clock
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

        // T�nh to�n gia t?c t?ng v� ki?m tra xem c� th? c� m?t bu?c ch�n kh�ng
        float total_acceleration = acceleration[0] + acceleration[1] + acceleration[2];
        if (total_acceleration > 1.5) {
            step_count++;
            LCD_I2C_DisplayNumber(step_count);
        }

        Delay_ms(10); // �?i trong m?t th?i gian ng?n tru?c khi d?c d? li?u m?i t? MPU6050
    }
}
