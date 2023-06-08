#include "main.h"
#include "lcd_i2c.h"
#include "delay.h"
#include "mpu6050.h"


float* CTR_MPU6050_getvalue;
char CTR_datasend[32];
uint8_t sw1 = 0;
uint8_t sw2 = 0;

float threshhold = 0.15;
float xval[100] = {0};
float yval[100] = {0};
float zval[100] = {0};
float xavg;
float yavg;
float zavg;
int flag = 0;

void config_led(void);
uint8_t state_interrupt_sw1(void);
void interrupt_init(uint32_t RCC_APB2Periph_GPIOX ,uint32_t GPIO_Pin_X, uint32_t GPIO_PortSourceGPIOX, 
uint32_t GPIO_PinSourceX, uint32_t EXTI_LineX, uint32_t EXTIX_IRQn,uint8_t NVIC_IRQChannelPreemptionPriorityX);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void calibrate(void);

int main(void)
{
	config_led();
	delay_init();
	
	interrupt_init(RCC_APB2Periph_GPIOB, GPIO_Pin_0, GPIO_PortSourceGPIOB, GPIO_PinSource0, 
	EXTI_Line0, EXTI0_IRQn, 0x00);
	
	interrupt_init(RCC_APB2Periph_GPIOB, GPIO_Pin_1, GPIO_PortSourceGPIOB, GPIO_PinSource1, 
	EXTI_Line1, EXTI1_IRQn, 0x01);
	
	I2C_MPU6050_Setup();
  I2C_MPU6050_Init();
	
	I2C_LCD_Setup();
	I2C_LCD_Init();
	I2C_LCD_Clear();
	sprintf(CTR_datasend,"So buoc : %d", count);
	I2C_LCD_Puts(CTR_datasend);
	sprintf(CTR_datasend,"Chua hoat dong");
	I2C_LCD_NewLine();
	I2C_LCD_Puts(CTR_datasend);
	I2C_LCD_BackLight(1);
	
	I2C_MPU6050_Setup();
	CTR_MPU6050_getvalue = CTR_READ_ACCEL_MPU6050();

	Ax = *(CTR_MPU6050_getvalue);
	Ay = *(CTR_MPU6050_getvalue + 1);
	Az = *(CTR_MPU6050_getvalue + 2);
	
	while (1) {
		I2C_LCD_BackLight(1);
		sw1 = state_interrupt_sw1();
		
		if (sw1 == 1)
		{
			calibrate();
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);
			I2C_MPU6050_Setup();
			CTR_MPU6050_getvalue = CTR_READ_ACCEL_MPU6050();
	 float totvect[100] = {0};
	 float totave[100] = {0};
	 float xaccl[100] = {0};
	 float yaccl[100] = {0};
	 float zaccl[100] = {0};

	 for (int i = 0; i < 100; i++) {
		xaccl[i] = *(CTR_MPU6050_getvalue);
		Delay_Ms(1);

		yaccl[i] = *(CTR_MPU6050_getvalue+1);
		Delay_Ms(1);

		zaccl[i] = *(CTR_MPU6050_getvalue+2);
		Delay_Ms(1);

		totvect[i] = sqrt(((xaccl[i] - xavg) * (xaccl[i] - xavg)) + ((yaccl[i] - yavg) * (yaccl[i] - yavg)) + ((zaccl[i] - zavg) * (zaccl[i] - zavg)));
		totave[i] = (totvect[i] + totvect[i - 1]) / 2 ;
		 
		Delay_Ms(200);

		if (totave[i] > threshhold && flag == 0) {
		 count++;
		 flag = 1;
			I2C_LCD_Setup();
			I2C_LCD_Clear();
			sprintf(CTR_datasend,"So buoc chan: %d", count);
			I2C_LCD_Puts(CTR_datasend);

		} else if (totave[i] > threshhold && flag == 1) {

		}

		if (totave[i] < threshhold  && flag == 1) {
		 flag = 0;
		}
	}
			
			if ( i == 200)
			GPIO_SetBits(GPIOA, GPIO_Pin_9);
			if (i == 400){
				GPIO_ResetBits(GPIOA, GPIO_Pin_9);
				i = 0;
			}
			
			i ++;
			Delay_Ms(1);
			
		} else {
			GPIO_SetBits(GPIOA, GPIO_Pin_8);
			GPIO_ResetBits(GPIOA, GPIO_Pin_9);
		}
		
	}
}

void EXTI0_IRQHandler()
{
		if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
      sw1 ++;
			if (sw1 % 2 == 1)
			{
				sw1 = 1;
			} else {
				sw1 = 0;
			}

				
     EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler()
{
		if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
				count = 0;
				I2C_LCD_Setup();
				I2C_LCD_Clear();
				sprintf(CTR_datasend,"So buoc: %d", count);
				I2C_LCD_Puts(CTR_datasend);
			
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void interrupt_init(uint32_t RCC_APB2Periph_GPIOX ,uint32_t GPIO_Pin_X, uint32_t GPIO_PortSourceGPIOX, 
	uint32_t GPIO_PinSourceX, uint32_t EXTI_LineX, uint32_t EXTIX_IRQn,uint8_t NVIC_IRQChannelPreemptionPriorityX)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Step1: Ennable clock cho GPIOX
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);

    // Step2: Config GPIOB_PIN_X as input pull-up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_X;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Step 3: Config ngat ngoài EXTIX
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOX, GPIO_PinSourceX);
		EXTI_InitStructure.EXTI_Line = EXTI_LineX;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
   
    // Step 4: Config uu tiên và cho phép ngat ngoài EXTIX
    NVIC_InitStructure.NVIC_IRQChannel = EXTIX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriorityX;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


uint8_t state_interrupt_sw1 ()
{
	return sw1;
}

void config_led(void) {
	GPIO_InitTypeDef gpioInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInit);
}

void calibrate() {
 float sum = 0;
 float sum1 = 0;
 float sum2 = 0;
 for (int i = 0; i < 100; i++) {
  xval[i] = (float)(Ax / 16384.0);
  sum = xval[i] + sum;
 }
 Delay_Ms(100);
 xavg = sum / 100.0;

 for (int j = 0; j < 100; j++) {
  xval[j] = (float)(Ay / 16384.0);

  sum1 = xval[j] + sum1;
 }
 yavg = sum1 / 100.0;
	Delay_Ms(100);

 for (int i = 0; i < 100; i++) {
  zval[i] = (float)(Az / 16384.0);

  sum2 = zval[i] + sum2;
 }
 zavg = sum2 / 100.0;
 Delay_Ms(100);
}
