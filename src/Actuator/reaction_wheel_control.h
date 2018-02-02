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
	/**
	 * @param rw	the reaction wheel actuator handler, needs to be initialized elsewhere
	 * @param enc	the encoder handler giving the lowpass filtered rps of the reaction wheel,
	 * 				needs to be initialized elsewhere
	 */
	ReactionWheelController(ReactionWheel *rw, Encoder *enc);

	/**
	 * Set the desired rps value that the reaction wheel should spin
	 * @param rps	the desired rotational velocity of the wheel
	 */
	void setDesiredRPS(float rps);

	/**
	 * call once in a time loop for the controller to work
	 *
	 * The controller itself is a combination of feed-forward and a basic p-controller
	 *
	 * @param int64_t beat: Specify period duration of loop iteration (Sample Time)
	 */
	void controlLoop(int64_t beat);
};


#endif /* ACTUATOR_REACTION_WHEEL_CONTROL_H_ */
