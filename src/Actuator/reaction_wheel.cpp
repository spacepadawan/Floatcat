/*
 * reaction_wheel.cpp
 *
 *  Created on: 29.11.2017
 *      Author: Hermann
 */


#include "reaction_wheel.h"

HAL_PWM pwm(RW_PWM_IDX);
HAL_GPIO in_a(A_PIN);
HAL_GPIO in_b(B_PIN);

void ReactionWheel::init() {
	pwm.init(RW_FREQUENCY, RW_INCREMENTS);
	in_a.init(true, 1, 0);
	in_b.init(true, 1, 0);
}

void ReactionWheel::setVelocity(float power) {
	if (MY_ABS(power) < 0.001) {
		in_a.setPins(0);
		in_b.setPins(0);
	} else {
		/*
		 * not sure about direction, depends on actual connection
		 */
		if (power > 0) {
			in_a.setPins(1);
			in_b.setPins(0);
		} else {
			in_a.setPins(0);
			in_b.setPins(1);
		}
		unsigned int inc = power * (float) RW_INCREMENTS;
		inc = inc > RW_INCREMENTS ? RW_INCREMENTS : inc;
		pwm.write(MY_ABS(power) * (float) RW_INCREMENTS);
		//pwm.write(4000);
	}
}
