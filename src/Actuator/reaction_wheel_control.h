/*
 * reaction_wheel_control.h
 *
 *  Created on: 15.12.2017
 *      Author: Hermann
 */

#ifndef ACTUATOR_REACTION_WHEEL_CONTROL_H_
#define ACTUATOR_REACTION_WHEEL_CONTROL_H_

#include "reaction_wheel.h"
#include "encoder.h"
#include "../topics.h"

class ReactionWheelController {

private:
	ReactionWheel *rw;
	Encoder *enc;
	float rps_des;
	float pwm_inf;

	float sum = 0;
	float sum_max = 50;

	float dif_old = 0;
	float dif = 0;

	PID_ControlParameters params;

public:
	ReactionWheelController(ReactionWheel *rw, Encoder *enc);

	void setDesiredRPS(float rps);
	void controlLoop(int64_t beat);
};


#endif /* ACTUATOR_REACTION_WHEEL_CONTROL_H_ */
