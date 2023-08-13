#include "MPU6050.h"
#include "util.h"

#define MPU6050_ADDRESS 0x68
#define PWR_MGMT_1 0x6B
#define USER_CTRL 0x6A
#define WHO_AM_I 0x75
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43
#define INT_ENABLE 0x38

#define GYRO_ADJUSTMENT 0.0005321126f

void MPU6050_Init(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c) {
	// MPU6050 setup
	//writeOneByte(i2c, MPU6050_ADDRESS, PWR_MGMT_1, 0x80);
	HAL_Delay(100);
	writeOneByte(i2c, MPU6050_ADDRESS, PWR_MGMT_1, 0x00);
	writeOneByte(i2c, MPU6050_ADDRESS, ACCEL_CONFIG, 0x10);
	writeOneByte(i2c, MPU6050_ADDRESS, GYRO_CONFIG, 0x10);
	writeOneByte(i2c, MPU6050_ADDRESS, INT_ENABLE, 0x00); // DATA_RDY_EN disable
	HAL_Delay(100);
	print(uart, "MPU-6050 Initialisation complete. (No self-test)\n");
}

void updateMPU6050(I2C_HandleTypeDef *i2c,
		struct AccCalibration *accCalibration,
		struct GyroCalibration *gyroCalibration) {

	updateAccelerometer(i2c);
	adjustAccelerometer(accCalibration);
	updateGyro(i2c);
	adjustGyro(gyroCalibration);
}

void updateAccelerometer(I2C_HandleTypeDef *i2c) {
	uint8_t data[6];
	readBytes(i2c, MPU6050_ADDRESS, ACCEL_XOUT_H, data, 6);

	int16_t x, y, z;
	x = (data[0] << 8) | data[1];
	y = (data[2] << 8) | data[3];
	z = (data[4] << 8) | data[5];

#if MPU6050_ORIENTATION == 0
	Ax = x * 8 * 9.81f / 32767.5f;
	Ay = y * 8 * 9.81f / 32767.5f;
	Az = z * 8 * 9.81f / 32767.5f;
#elif MPU6050_ORIENTATION == 180
	Ax = -x * 8 * 9.81f / 32767.5f;
	Ay = -y * 8 * 9.81f / 32767.5f;
	Az = z * 8 * 9.81f / 32767.5f;
#else
#error "MPU orientation not implemented yet."
#endif
}

void updateGyro(I2C_HandleTypeDef *i2c) {
	uint8_t data[6];
	readBytes(i2c, MPU6050_ADDRESS, GYRO_XOUT_H, data, 6);

	int16_t x, y, z;
	x = (data[0] << 8) | data[1];
	y = (data[2] << 8) | data[3];
	z = (data[4] << 8) | data[5];

	// (G / 32.8 (sensitivity))* PI / 180.0f
#if MPU6050_ORIENTATION == 0
	Gx = x * GYRO_ADJUSTMENT;
	Gy = y * GYRO_ADJUSTMENT;
	Gz = z * GYRO_ADJUSTMENT;
#elif MPU6050_ORIENTATION == 180
	Gx = -x * GYRO_ADJUSTMENT;
	Gy = -y * GYRO_ADJUSTMENT;
	Gz = z * GYRO_ADJUSTMENT;
#elif MPU6050_ORIENTATION == 180
#else
#error "MPU orientation not implemented yet."
#endif
}

void adjustAccelerometer(struct AccCalibration *accCal) {
	Ax = 9.81
			* (2 * (Ax - accCal->AxMin) / (accCal->AxMax - accCal->AxMin) - 1);
	Ay = 9.81
			* (2 * (Ay - accCal->AyMin) / (accCal->AyMax - accCal->AyMin) - 1);
	Az = 9.81
			* (2 * (Az - accCal->AzMin) / (accCal->AzMax - accCal->AzMin) - 1);
}

void adjustGyro(struct GyroCalibration *gyroCal) {
	Gx = Gx - gyroCal->GxOff;
	Gy = Gy - gyroCal->GyOff;
	Gz = Gz - gyroCal->GzOff;
}

void calibrateGyro(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c) {
	while (1) {
		float GxAvg = 0;
		float GyAvg = 0;
		float GzAvg = 0;

		for (int i = 0; i < 1000; i++) {
			updateGyro(i2c);
			GxAvg += Gx / 1000.0f;
			GyAvg += Gy / 1000.0f;
			GzAvg += Gz / 1000.0f;
		}

		printFloats(uart, "", 3, GxAvg * 1000.0f, GyAvg * 1000.0f,
				GzAvg * 1000.0f);
	}
}

void calibrateAccelerometer(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c) {
	while (1) {
		updateAccelerometer(i2c);
		printFloats(uart, "", 3, Ax, Ay, Az);
	}
}

// 0x68
uint8_t getMPUWhoAmI(I2C_HandleTypeDef *i2c) {
	uint8_t res;
	readBytes(i2c, MPU6050_ADDRESS, WHO_AM_I, &res, 1);
	return res;
}

