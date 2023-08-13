#include "QMC5883L.h"
#include "util.h"

#define QMC_ADDRESS 0x0D
#define WHOIS_REGISTER 0x0D
#define XOUT_REGISTER 0x00
#define CR1_REGISTER 0x09
#define CR2_REGISTER 0x0A

void QMC5883L_Init(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c,
		uint8_t rateSetting) {
	// 0000RR01 10:00 50:01 100:10 200:11
	writeOneByte(i2c, QMC_ADDRESS, CR1_REGISTER,
			0b00000001 | (rateSetting << 2));

	print(uart, "QMC5883L initialisation complete.\n");
}

void updateQMC5883L(I2C_HandleTypeDef *i2c,
		struct MagCalibration *magCalibration, GPIO_TypeDef *rdyClass,
		uint16_t rdyPin) {

	GPIO_PinState rdy = HAL_GPIO_ReadPin(rdyClass, rdyPin);
	if (rdy == GPIO_PIN_SET) {
		updateMagnetometer(i2c);
		adjustMagnetometer(magCalibration);
	}
}

void updateMagnetometer(I2C_HandleTypeDef *i2c) {
	uint8_t data[6];
	readBytes(i2c, QMC_ADDRESS, XOUT_REGISTER, data, 6);

	int16_t Lx = (data[1] << 8) | data[0];
	int16_t Ly = (data[3] << 8) | data[2];
	int16_t Lz = (data[5] << 8) | data[4];

#if QMC5883L_ORIENTATION == 90
	Mx = Lx;
	My = Ly;
	Mz = Lz;
#elif QMC5883L_ORIENTATION == 180
	Mx = -Ly;
	My = Lx;
	Mz = Lz;
#else
#error "Orientation not yet implemented"
#endif
}

void adjustMagnetometer(struct MagCalibration *cal) {
	Mx = 2 * (Mx - cal->MxMin) / (cal->MxMax - cal->MxMin) - 1;
	My = 2 * (My - cal->MyMin) / (cal->MyMax - cal->MyMin) - 1;
	Mz = 2 * (Mz - cal->MzMin) / (cal->MzMax - cal->MzMin) - 1;
}

void calibrateMagnetometer(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c, GPIO_TypeDef *rdyClass, uint16_t rdyPin) {
	while (1) {
		GPIO_PinState rdy = HAL_GPIO_ReadPin(rdyClass, rdyPin);
		if (rdy == GPIO_PIN_SET) {
			updateMagnetometer(i2c);
			printFloats(uart, "", 3, Mx / 1000.0f, My / 1000.0f, Mz / 1000.0f);
		}
	}
}

uint8_t getWhoIs(I2C_HandleTypeDef *i2c) {
	uint8_t id;
	readBytes(i2c, QMC_ADDRESS, WHOIS_REGISTER, &id, 1);
	return id;
}
