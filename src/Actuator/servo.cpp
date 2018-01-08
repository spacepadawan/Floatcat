/*
 * servo.cpp
 *
 *  Created on: 18.12.2017
 *      Author: Hermann
 */

#include "servo.h"

Servo::Servo(PWM_IDX idx) : pwm(idx) {
	min = -30;
	max = 30;
}

void Servo::init(float min, float max) {
	pwm.init(SERVO_FREQUENCY, SERVO_INCREMENTS);
	this->min = min;
	this->max = max;
	minBoundary = MIN_INC;
	maxBoundary = MAX_INC;
}

void Servo::setBoundaries(float min, float max) {
	if (min > max) return;

	int minBoundary_temp = angleToInc(min);
	int maxBoundary_temp = angleToInc(max);

	if (minBoundary_temp > MAX_INC) return;
	if (maxBoundary_temp < MIN_INC) return;

	minBoundary = minBoundary_temp > MIN_INC ? minBoundary_temp : MIN_INC;
	maxBoundary = maxBoundary_temp < MAX_INC ? maxBoundary_temp : MAX_INC;
}

int Servo::angleToInc(float deg) {
	return INC_AT_ZERO
			+ ((float) ((deg - min) / (max - min))
					* (INC_AT_MAX - INC_AT_ZERO));
}

void Servo::write(float deg) {
	writeInc(angleToInc(deg));
}

void Servo::writeInc(int inc) {
	inc = inc > MAX_INC ? MAX_INC : inc;
	inc = inc < MIN_INC ? MIN_INC : inc;

	inc = inc > maxBoundary ? maxBoundary : inc;
	inc = inc < minBoundary ? minBoundary : inc;
	pwm.write(inc);
}
