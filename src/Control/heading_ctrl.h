/*
 * heading_ctrl.h
 *
 *  Created on: 07.02.2018
 *      Author: Hermann
 */

#ifndef CONTROL_HEADING_CTRL_H_
#define CONTROL_HEADING_CTRL_H_

#include "rodos.h"
#include "../topics.h"

class HeadingCtrl {

private:
	//Pose sum = {0};
	Pose old;

	float sum_phi;


	ControlDebug debug;

public:

	void init() {
		PID_ControlParameters heading_params;
		heading_params.kp = 1;
		heading_params.kd = 0;
		heading_params.ki = 0;
		heading_ctrl_params.publish(heading_params);
	}

	void loop(int64_t dt, const Pose &pose, const Pose &poseDot, const Pose &desired,
			PID_ControlParameters heading_params) {
		float deltaT = dt / SECONDS;

		Pose error = desired - pose;

		debug.e_x = 0;
		debug.e_y = 0;
		debug.e_phi = error.phi;

		sum_phi += error.phi * deltaT;

		float m = heading_params.kp * error.phi - heading_params.kd * poseDot.phi + heading_params.ki * sum_phi;
		desiredRotationSpeedBuffer.put(m);

		debug.tx = 0;
		debug.ty = 0;
		debug.m = m;

		controlDebugBuffer.put(debug);

		old = pose;
	}


};


#endif /* CONTROL_HEADING_CTRL_H_ */
