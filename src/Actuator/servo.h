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
	uint16_t minBoundary, maxBoundary;

	int angleToInc(float deg);
public:
	/**
	 * @param idx	Use PWM_IDX00, PWM_IDX01, PWM_IDX02 or PWM_IDX03 and connect the servos to the extension board accordingly
	 */
	Servo(PWM_IDX idx);

	/**
	 * initialize the servo with its nominal angles at INC_AT_ZERO and INC_AT_MAX
	 */
	void init(float min, float max);

	/**
	 * set boundaries to the servo handler to prevent the servo from turning into physically unreachable positions (i.e. due to obstructions)
	 */
	void setBoundaries(float min, float max);

	/**
	 * set a specified angle for the servos,
	 * @param deg	angle in degree, [-30, +30]
	 */
	void write(float deg);

	/**
	 * directly write the pwm increments to the servo motor, this can't bypass the set boundaries
	 */
	void writeInc(int inc);

	float getMax() { return max; }
	float getMin() { return min; }
};


#endif /* ACTUATOR_SERVO_H_ */
