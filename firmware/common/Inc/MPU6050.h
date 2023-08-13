#pragma once
#include <stdint.h>

#include "board_properties.h"
#ifdef STM32G0
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_i2c.h"
#include "stm32g0xx_hal_uart.h"
#include "stm32g0xx_hal_conf.h"
#endif
#ifdef STM32F0
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_uart.h"
#include "stm32f0xx_hal_conf.h"
#endif

float Ax, Ay, Az;
float Gx, Gy, Gz;

// These offsets are as reported by "calibrateGyro" and must be divided by 1e6.
struct GyroCalibration {
	float GxOff;
	float GyOff;
	float GzOff;
};

struct AccCalibration {
	float AxMin;
	float AxMax;
	float AyMin;
	float AyMax;
	float AzMin;
	float AzMax;
};

void MPU6050_Init(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c);

void updateMPU6050(I2C_HandleTypeDef *i2c,
		struct AccCalibration *accCalibration,
		struct GyroCalibration *gyroCalibration);
void updateAccelerometer(I2C_HandleTypeDef *i2c);
void updateGyro(I2C_HandleTypeDef *i2c);
void adjustAccelerometer(struct AccCalibration *accCal);
void adjustGyro(struct GyroCalibration *gyroCal);
void calibrateGyro(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c);
void calibrateAccelerometer(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c);

// 0x68
uint8_t getMPUWhoAmI(I2C_HandleTypeDef *i2c);

