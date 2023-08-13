#pragma once

#include "board_properties.h"
#ifdef STM32G0
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_i2c.h"
#include "stm32g0xx_hal_uart.h"
#endif
#ifdef STM32F0
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_uart.h"
#endif

#define RATE_10HZ 0b00
#define RATE_50HZ 0b01
#define RATE_100HZ 0b10
#define RATE_200HZ 0b11

float Mx, My, Mz;

struct MagCalibration {
	float MxMin;
	float MxMax;
	float MyMin;
	float MyMax;
	float MzMin;
	float MzMax;
};

// Rates are from the following: RATE_10HZ, RATE_50HZ, RATE_100HZ, RATE_200HZ
void QMC5883L_Init(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c,
		uint8_t rateSetting);

void updateQMC5883L(I2C_HandleTypeDef *i2c,
		struct MagCalibration *magCalibration, GPIO_TypeDef *rdyClass,
		uint16_t rdyPin);

void updateMagnetometer(I2C_HandleTypeDef *i2c);
void adjustMagnetometer(struct MagCalibration *cal);

void calibrateMagnetometer(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c, GPIO_TypeDef *rdyClass, uint16_t rdyPin);

uint8_t getWhoIs();
