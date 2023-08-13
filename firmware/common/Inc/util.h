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

#define PI 3.141592653f

//float toDeg = 180.0f / PI;

void printFloats(UART_HandleTypeDef *huart, char *str, int n, ...);
void print(UART_HandleTypeDef *huart, char *str);
void printTimer(UART_HandleTypeDef *huart, uint16_t i);
void printQuaternion(UART_HandleTypeDef *huart, double w, double v[]);
void printTimerInv(UART_HandleTypeDef *huart, uint16_t i);
float arctan2(float a, float b);
float boundOne(float a);

void readBytes(I2C_HandleTypeDef *i2c, uint8_t address, uint8_t subaddress,
		uint8_t *data, uint8_t size);
void writeOneByte(I2C_HandleTypeDef *i2c, uint8_t address, uint8_t subaddress,
		uint8_t data);

void ledflash(I2C_HandleTypeDef *i2c);

int add(int a, int b);