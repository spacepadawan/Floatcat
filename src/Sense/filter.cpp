/*
 * filter.cpp
 *
 *  Created on: 19.12.2017
 *      Author: Hermann
 */

#include "filter.h"


Pose Filter::filterPose(SensorData* sd, uint64_t dt) {

	float deltaT = (float) dt / SECONDS;

	angularRates[currentSumIndex] = sd->gyroscope.z;
	angularRateSum += sd->gyroscope.z;
	currentSumIndex = (currentSumIndex + 1) % LOWPASS_SAMPLES;
	angularRateSum -= angularRates[currentSumIndex];

	poseDot.phi = angularRateSum / LOWPASS_SAMPLES;
	//angularRateTopic.publish(angularRateLowpass);

	poseDotTopic.publish(poseDot);


	//float heading = pose.phi + sd->gyroscope.z * deltaT;

	/*
	//Filter using magnetometer

	float magHeading = 180 * atan2f(sd->magnetometer.x, sd->magnetometer.y) / M_PI;
	float gyroHeading = heading + sd->gyroscope.z * deltaT;

	float dif = magHeading - gyroHeading;

	if (dif >= 180) {
		magHeading -= 360;
	} else if (dif < -180) {
		gyroHeading -= 360;
	}

	heading = magHeading * factor + gyroHeading * (1 - factor);

	*/

	pose.phi += sd->gyroscope.z * deltaT;

	if (pose.phi >= 180.0) pose.phi -= 360;
	if (pose.phi < -180.0) pose.phi += 360;

	//Filter using accelerometer
	float s = sin(pose.phi * M_PI / 180);
	float c = cos(pose.phi * M_PI / 180);

	float ax = - c * sd->accelerometer.x + s * sd->accelerometer.y;
	float ay = - c * sd->accelerometer.y - s * sd->accelerometer.x;

	//just remove this line if the accelerometer should be used
	//ax = ay = 0;

	//vx += ax * deltaT * 9.81;
	//vy += ay * deltaT * 9.81;

	//pose.x += vx * deltaT;
	//pose.y += vy * deltaT;

	return pose;
}

void Filter::updatePose(Pose p) {
	int64_t t = NOW();
	int64_t timeDif = t - updateTime;

	oldPose = pose;
	pose = p;

	if (timeDif > 2 * SECONDS || timeDif < 100 * MILLISECONDS) {
		vx = 0;
		vy = 0;
	} else {
		poseDot.x = (pose.x - oldPose.x) /  ((float) timeDif / (float) SECONDS);
		poseDot.y = (pose.y - oldPose.y) /  ((float) timeDif / (float) SECONDS);
	}

	//poseDot.x = poseDot.x > 0.5 ? 0.5 : poseDot.x;
	//poseDot.x = poseDot.x < -0.5 ? -0.5 : poseDot.x;
	//poseDot.y = poseDot.y > 0.5 ? 0.5 : poseDot.x;
	//poseDot.y = poseDot.y < -0.5 ? -0.5 : poseDot.x;

	// delete if necessary
	//vx = vy = 0;

	//PRINTF("v: %f, %f", vx, vy);

	updateTime = t;
}
