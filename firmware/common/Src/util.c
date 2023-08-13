#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "util.h"

void printFloats(UART_HandleTypeDef *huart, char *str, int n, ...) {
	HAL_UART_Transmit(huart, (uint8_t*) str, strlen(str), 1000);
	va_list list;
	va_start(list, n);
	int current;
	char output[10];

	// Most
	for (int i = 0; i < n - 1; i++) {
		current = (int) (1000 * ((float) va_arg(list, double)));
		snprintf(output, 10, "%d,", current);
		HAL_UART_Transmit(huart, (uint8_t*) output, strlen(output), 1000);
	}

	// Final
	current = (int) (1000 * ((float) va_arg(list, double)));
	snprintf(output, 10, "%d\n", current);
	HAL_UART_Transmit(huart, (uint8_t*) output, strlen(output), 1000);
	va_end(list);
}

void print(UART_HandleTypeDef *huart, char *str) {
	HAL_UART_Transmit(huart, (uint8_t*) str, strlen(str), 1000);
}

void printQuaternion(UART_HandleTypeDef *huart, double w, double v[]) {
	printFloats(huart, "", 4, (float) w, (float) v[0], (float) v[1],
			(float) v[2]);
}

void printTimer(UART_HandleTypeDef *huart, uint16_t i) {
	char output[50];
	int len = sprintf(output, "Time: %u us\n", i);
	HAL_UART_Transmit(huart, (uint8_t*) output, len, 1000);
}

void printTimerInv(UART_HandleTypeDef *huart, uint16_t i) {
	float freq = 1.0f / 100.0f / (i / 1000000.0f);
	printFloats(huart, "Freq (Hz): ", 1, freq);
}

// Magic...
// https://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/
float arctan2(float y, float x) {
	float c1 = PI / 4.0f;
	float c2 = 3 * c1;
	float abs_y = fabs(y) + 1e-10f;
	float angle;

	if (x >= 0) {
		float r = (x - abs_y) / (x + abs_y);
		angle = c1 - c1 * r;
	} else {
		float r = (x + abs_y) / (abs_y - x);
		angle = c2 - c1 * r;
	}
	if (y < 0)
		return -angle;
	else
		return angle;
}

float boundOne(float a) {
	if (a < -1.0f)
		return -1.0f;
	else if (a > 1.0f)
		return 1.0f;
	else
		return a;
}

void readBytes(I2C_HandleTypeDef *i2c, uint8_t address, uint8_t subaddress,
		uint8_t *data, uint8_t size) {
	HAL_I2C_Mem_Read(i2c, address << 1, subaddress, I2C_MEMADD_SIZE_8BIT, data,
			size, HAL_MAX_DELAY);
}

void writeOneByte(I2C_HandleTypeDef *i2c, uint8_t address, uint8_t subaddress,
		uint8_t data) {
	HAL_I2C_Mem_Write(i2c, address << 1, subaddress, I2C_MEMADD_SIZE_8BIT,
			&data, 1, HAL_MAX_DELAY);
}

void ledflash(I2C_HandleTypeDef *i2c) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_Delay(500);
}

int add(int a, int b) {
	return a + b;
}