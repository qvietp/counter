#include "stm32f10x.h"
#include "mpu6050.h"
#include "lcd_i2c.h"

void SysTick_Handler(void)
{
    static uint16_t step_count = 0;
    float acceleration[3];
    MPU6050_GetAcceleration(acceleration);

    float total_acceleration = acceleration[0] + acceleration[1] + acceleration[2];
    if (total_acceleration > 1.5) {
        step_count++;
        LCD_I2C_DisplayNumber(step_count);
    }
}
int main(void)
{
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	  SysTick_Config(SystemCoreClock/1000);
    uint16_t step_count = 0;
    float acceleration[3];

    MPU6050_Init();
    LCD_I2C_Init();

    while (1)
    {
        SysTick_Handler();
    }
}
