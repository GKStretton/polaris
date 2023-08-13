#pragma once

#include "stm32g0xx_hal.h"

void Polaris_Init(UART_HandleTypeDef *huart1);
void Polaris_Update(UART_HandleTypeDef *huart1);