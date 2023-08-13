#include "compFilter.h"
#include "math.h"
#include "util.h"

int firstUpdate = 1;
float alphaAcc = 0.99f;
float alphaMag = 0.99f;

void updateCompFilter(float timeElapsed, float Gx, float Gy, float Gz, float Ax,
		float Ay, float Az, float Mx, float My, float Mz) {
	Rz += timeElapsed
			* (Gz + Gx * sinf(Rz) * sinf(Rx) / cosf(Rx)
					+ Gy * cosf(Rz) * sinf(Rx) / cosf(Rx));
	Rx += timeElapsed * (Gx * cosf(Rz) - Gy * sinf(Rz));
	Ry += timeElapsed * (Gx * sinf(Rz) / cosf(Rx) + Gy * cosf(Rz) / cosf(Rx));

	float Apitch = arctan2(-Az, Ay);
	float Aroll = arctan2(Ax, Ay);
	Rx = Rx * alphaAcc + Apitch * (1 - alphaAcc);
	Rz = Rz * alphaAcc + Aroll * (1 - alphaAcc);

	if (firstUpdate) {
		firstUpdate = 0;
		Rx = Apitch;
		Rz = Aroll;
	}

	if (Ry > PI) {
		Ry -= 2 * PI;
	} else if (Ry < -PI) {
		Ry += 2 * PI;
	}

	/*
	 float x = Mx * cosf(-Aroll) + My * sinf(-Aroll);
	 float z = Mx * sinf(-Aroll) * sinf(-Apitch) - My * cosf(-Aroll) * sinf(-Apitch) + Mz * cosf(-Apitch);
	 float heading = -arctan2(-x, -z);

	 if (heading < PI * 0.8 && heading > -PI * 0.8 && Ry < PI * 0.8 && Ry > -PI * 0.8) {
	 Ry = Ry * alphaMag + heading * (1 - alphaMag);
	 }*/
}
