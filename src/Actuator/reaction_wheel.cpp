/*
 * reaction_wheel.cpp
 *
 *  Created on: 29.11.2017
 *      Author: Hermann
 */

#include "reaction_wheel.h"

ReactionWheel::ReactionWheel(HAL_PWM* pwm, HAL_GPIO* in_a, HAL_GPIO* in_b) {
	this->pwm = pwm;
	this->in_a = in_a;
	this->in_b = in_b;
}

void ReactionWheel::init() {
	pwm->init(RW_FREQUENCY, RW_INCREMENTS);
	in_a->init(true, 1, 0);
	in_b->init(true, 1, 0);
}

void ReactionWheel::setPower(float power) {
	if (MY_ABS(power) < 0.001) {
		in_a->setPins(0);
		in_b->setPins(0);
		pwm->write(0);
	} else {
		/*
		 * not sure about direction, depends on actual connection
		 */
		if (power > 0) {
			in_a->setPins(1);
			in_b->setPins(0);
		} else {
			in_a->setPins(0);
			in_b->setPins(1);
		}
		unsigned int inc = MY_ABS(power) * (float) RW_INCREMENTS;
		inc = inc > RW_MAX_INCREMENTS ? RW_MAX_INCREMENTS : inc;
		pwm->write(inc);
	}
}

