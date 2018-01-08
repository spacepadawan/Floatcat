/*
 * filter.cpp
 *
 *  Created on: 19.12.2017
 *      Author: Hermann
 */

#include "filter.h"


float Filter::filterHeading(SensorData* sd, uint64_t dt) {
	float magHeading = 180 * atan2f(sd->magnetometer.x, sd->magnetometer.y) / M_PI;
	float gyroHeading = heading + sd->gyroscope.z * ((float) dt / SECONDS);

	float dif = magHeading - gyroHeading;

	if (dif >= 180) {
		magHeading -= 360;
	} else if (dif < -180) {
		gyroHeading -= 360;
	}

	heading = magHeading * factor + gyroHeading * (1 - factor);

	if (heading >= 180.0) heading -= 360;
	if (heading < -180.0) heading += 360;

	return heading;
}
