#ifndef MPU6050_H_
#define MPU6050_H_

#include "stm32f10x.h"

#define MPU6050_ADDR 0xD0
#define MPU6050_ACCEL_SCALE_2G 16384.0
#define MPU6050_ACCEL_SCALE_4G 8192.0
#define MPU6050_ACCEL_SCALE_8G 4096.0
#define MPU6050_ACCEL_SCALE_16G 2048.0

void MPU6050_Init(void);
void MPU6050_GetAcceleration(float* acceleration);

#endif /* MPU6050_H_ */
