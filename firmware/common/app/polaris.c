#include "app/polaris.h"
#include "stdint.h"
#include "stm32g071xx.h"
#include "util/util.h"

void Polaris_Init(UART_HandleTypeDef *huart1) {
    char *start = "Start up\n";
    HAL_UART_Transmit(huart1, (uint8_t*) start, strlen(start), 1000);
}

void Polaris_Update(UART_HandleTypeDef *huart1) {
    HAL_Delay(add(50, 50));
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    char *hi = "Hi there\n";
    HAL_UART_Transmit(huart1, (uint8_t*) hi, strlen(hi), 1000);
}