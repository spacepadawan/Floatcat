/*
 * reaction_wheel_control.cpp
 *
 *  Created on: 15.12.2017
 *      Author: Hermann
 */

#include "reaction_wheel_control.h"

ReactionWheelController::ReactionWheelController(ReactionWheel *rw, Encoder *enc) : rw(rw), enc(enc) {
	rps_des = 0;
	pwm_inf = 0;
	//default params:
	params.kp = 0.1;
	params.kd = 0;
	params.ki = 0;//0.0035;
	rw_ctrl_params.publish(params);
}

void ReactionWheelController::setDesiredRPS(float rps) {
	rps_des = rps;
	pwm_inf = rps * rps * 0.00005 + 0.1; // This parameter determines the pwm signal needed to hold up a certain rps
	pwm_inf = pwm_inf < 0.105 ? 0 : pwm_inf;
	pwm_inf *= rps < 0 ? -1 : 1;
}

void ReactionWheelController::controlLoop(int64_t beat) {

	dif = rps_des - enc->get_rps_lowpass(beat); // control error

	/*float delta_dif = (dif - dif_old) / ((float) beat / (float) SECONDS);

	if (dif / sum < 0) {
		sum = 0;
	}

	sum += dif * ((float) beat / (float) SECONDS);

	sum = sum > sum_max ? sum_max : sum;
	sum = sum < -sum_max ? -sum_max : sum;
*/

	//rw_ctrl_params_buffer.get(params);


	float output = pwm_inf + params.kp * dif;// + params.kd * delta_dif + params.ki * sum;

	rw->setPower(output);

	dif_old = dif;
}


