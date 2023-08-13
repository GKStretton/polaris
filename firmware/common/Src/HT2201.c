#include "HT2201.h"
#include "util.h"

#define HT2201_ADDRESS 0x50

uint8_t readWord(I2C_HandleTypeDef *i2c, uint8_t addr) {
	uint8_t res;
	readBytes(i2c, HT2201_ADDRESS, addr, &res, 1);
	return res;
}

float readFloat(I2C_HandleTypeDef *i2c, uint8_t addr) {
	return 0.0f;
}

void writeWord(I2C_HandleTypeDef *i2c, uint8_t addr, uint8_t word) {
	writeOneByte(i2c, HT2201_ADDRESS, addr, word);
}

void writeFloat(I2C_HandleTypeDef *i2c, uint8_t addr, float number) {

}
