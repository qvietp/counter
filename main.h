#include "stm32f10x.h"
#include "stdio.h"
#include "math.h" 


void config_led(void);
uint8_t state_interrupt_sw1(void);
void interrupt_init(uint32_t RCC_APB2Periph_GPIOX ,uint32_t GPIO_Pin_X, uint32_t GPIO_PortSourceGPIOX, 
uint32_t GPIO_PinSourceX, uint32_t EXTI_LineX, uint32_t EXTIX_IRQn,uint8_t NVIC_IRQChannelPreemptionPriorityX);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void calibrate(void);