#include "mpu6050.h"
#include "i2c.h"

void MPU6050_Init(void)
{
    // Kích hoat nguon MPU6050
    uint8_t turn_on[] = {0x6B, 0x00};
    I2C_Write(MPU6050_ADDR, turn_on, sizeof(turn_on));

    // Cau hình giá tri scale cho gia toc
    uint8_t set_scale[] = {0x1C, 0x00};
    I2C_Write(MPU6050_ADDR, set_scale, sizeof(set_scale));
}

void MPU6050_GetAcceleration(float* acceleration)
{
    uint8_t buffer[6];

    // Ðoc giá tri do duoc tu MPU6050
    uint8_t read_addr = 0x3B;
    I2C_Read(MPU6050_ADDR, &read_addr, 1, buffer, 6);

    // Chuyen doi giá tri do duoc thành don vi m/s^2
    acceleration[0] = (float)((int16_t)((buffer[0] << 8) | buffer[1])) / MPU6050_ACCEL_SCALE_2G * 9.81;
    acceleration[1] = (float)((int16_t)((buffer[2] << 8) | buffer[3])) / MPU6050_ACCEL_SCALE_2G * 9.81;
    acceleration[2] = (float)((int16_t)((buffer[4] << 8) | buffer[5])) / MPU6050_ACCEL_SCALE_2G * 9.81;
}
