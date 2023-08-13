#pragma once

#include "board_properties.h"

#include <stdint.h>

#ifdef STM32G0
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_i2c.h"
#endif
#ifdef STM32F0
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"
#endif

uint8_t readWord(I2C_HandleTypeDef *i2c, uint8_t addr);
float readFloat(I2C_HandleTypeDef *i2c, uint8_t addr);
void writeWord(I2C_HandleTypeDef *i2c, uint8_t addr, uint8_t word);
void writeFloat(I2C_HandleTypeDef *i2c, uint8_t addr, float number);
