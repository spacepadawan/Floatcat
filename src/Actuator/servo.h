/*
 * servo.h
 *
 *  Created on: 23.11.2017
 *      Author: Hermann
 */

#ifndef ACTUATOR_SERVO_H_
#define ACTUATOR_SERVO_H_

#include "rodos.h"

//#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
//#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define SERVO_INCREMENTS    		20000     // maximum increments
#define SERVO_FREQUENCY			   50	  // pwm frequency
#define INC_AT_ZERO			1000
#define INC_AT_MAX			2000
#define MIN_INC				850
#define MAX_INC				2150


class Servo {
private:
	HAL_PWM pwm;
	float min, max;
public:
	/**
	 * @param idx	Use PWM_IDX00, PWM_IDX01, PWM_IDX02 or PWM_IDX03 and connect the servos to the extension board accordingly
	 */
	Servo(PWM_IDX idx) : pwm(idx) {
		min = -30;
		max = 30;
	}

	void init(float min, float max) {
		pwm.init(SERVO_FREQUENCY, SERVO_INCREMENTS);
		this->min = min;
		this->max = max;
	}

	/**
	 * @param deg	angle in degree, [-30, +30]
	 */
	void write(float deg) {
		int inc = INC_AT_ZERO + ((float) ((deg - min) / (max - min)) * (INC_AT_MAX - INC_AT_ZERO));
		writeInc(inc);
	}

	void writeInc(int inc) {
		inc = inc > MAX_INC ? MAX_INC : inc;
		inc = inc < MIN_INC ? MIN_INC : inc;
		pwm.write(inc);
	}

	float getMax() { return max; }
	float getMin() { return min; }
};


#endif /* ACTUATOR_SERVO_H_ */
