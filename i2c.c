#include "i2c.h"

int I2C_TIMEOUT = 100000;

void I2C_Init1(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    // Kh?i t?o GPIO và I2C
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStruct);

    I2C_Cmd(I2C1, ENABLE);
}

void I2C_Write(uint8_t addr, uint8_t* data, uint16_t size)
{
    // G?i l?nh START
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((I2C_TIMEOUT--) == 0) return;
    }
    I2C_TIMEOUT = 100000;

    // G?i d?a ch? thi?t b? và ghi d? li?u
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((I2C_TIMEOUT--) == 0) return;
    }
    I2C_TIMEOUT = 100000;

    for (int i = 0; i < size; i++) {
        I2C_SendData(I2C1, data[i]);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if ((I2C_TIMEOUT--) == 0) return;
        }
        I2C_TIMEOUT = 100000;
    }

    // G?i l?nh STOP
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_Read(uint8_t addr, uint8_t* read_addr, uint8_t read_addr_size, uint8_t* buffer, uint16_t size)
{
    // G?i l?nh START
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((I2C_TIMEOUT--) == 0) return;
    }
    I2C_TIMEOUT = 100000;

    // G?i d?a ch? thi?t b? và ghi d?a ch? b?t d?u d?c
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((I2C_TIMEOUT--) == 0) return;
    }
    I2C_TIMEOUT = 100000;

    for (int i = 0; i < read_addr_size; i++) {
        I2C_SendData(I2C1, read_addr[i]);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if ((I2C_TIMEOUT--) == 0) return;
        }
        I2C_TIMEOUT = 100000;
    }

    // G?i l?nh RESTART
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((I2C_TIMEOUT--) == 0) return;
    }
    I2C_TIMEOUT = 100000;

    // G?i d?a ch? thi?t b? và d?c d? li?u
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if ((I2C_TIMEOUT--) == 0) return;
    }
    I2C_TIMEOUT = 100000;

    for (int i = 0; i < size; i++)
    {
        if (i == size - 1)
        {
            // G?i l?nh STOP tru?c khi d?c d?n byte cu?i cùng
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            if ((I2C_TIMEOUT--) == 0) return;
        }
        I2C_TIMEOUT = 100000;
        buffer[i] = I2C_ReceiveData(I2C1);
    }
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}
