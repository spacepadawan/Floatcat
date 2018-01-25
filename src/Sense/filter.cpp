/*
 * filter.cpp
 *
 *  Created on: 19.12.2017
 *      Author: Hermann
 */

#include "filter.h"


Pose Filter::filterPose(SensorData* sd, uint64_t dt) {

	float deltaT = (float) dt / SECONDS;

	//Filter using magnetometer
	float heading = pose.phi;
	float magHeading = 180 * atan2f(sd->magnetometer.x, sd->magnetometer.y) / M_PI;
	float gyroHeading = heading + sd->gyroscope.z * deltaT;

	float dif = magHeading - gyroHeading;

	if (dif >= 180) {
		magHeading -= 360;
	} else if (dif < -180) {
		gyroHeading -= 360;
	}

	heading = magHeading * factor + gyroHeading * (1 - factor);

	if (heading >= 180.0) heading -= 360;
	if (heading < -180.0) heading += 360;

	pose.phi = heading;

	//Filter using accelerometer
	float s = sin(pose.phi * M_PI / 180);
	float c = cos(pose.phi * M_PI / 180);
	vx += c * sd->accelerometer.x * deltaT - s * sd->accelerometer.y * deltaT / 9.81;
	vy += c * sd->accelerometer.y * deltaT + s * sd->accelerometer.x * deltaT / 9.81;

	pose.x += vx * deltaT;
	pose.y += vy * deltaT;

	return pose;
}

void Filter::updatePose(Pose p) {
	int64_t t = NOW();
	int64_t timeDif = t - updateTime;

	oldPose = pose;
	pose = p;

	if (timeDif > 1 * SECONDS) {
		vx = 0;
		vy = 0;
	} else {
		vx = (pose.x - oldPose.x) / (timeDif / SECONDS);
		vy = (pose.y - oldPose.y) / (timeDif / SECONDS);
	}

	updateTime = t;
}
