/*
 * angularRateControl.h
 *
 *  Created on: 30.01.2018
 *      Author: Hermann
 */

#ifndef CONTROL_ANGULARRATECONTROL_H_
#define CONTROL_ANGULARRATECONTROL_H_

#include "rodos.h"
#include "../topics.h"

class RPMControl {

	float sum = 0;
	float lastError = 0;
	float dif = 0;

	PID_ControlParameters params;

public:

	RPMControl() {

	}

	void init() {
		params.kp = 2;
		params.kd = 0;
		params.ki = 0.0001;
		angular_ctrl_params.publish(params);
	}

	void loop(int64_t dt) {
		float deltaT = (float) dt / (float) SECONDS;
		float desired = 0;
		desiredRotationSpeedBuffer.get(desired);

		Pose poseDot;
		poseDotBuffer.get(poseDot);

		//angularRateBuffer.get(angularRate);

		//PRINTF("angularRate: %f ", angularRate);

		float ctrl_error =  poseDot.phi - desired;

		sum += ctrl_error * deltaT;

		if ((ctrl_error >= 0 && lastError <= 0) || (ctrl_error <= 0 && lastError >= 0)) {
			sum = 0;
		}

		dif = (ctrl_error - lastError) / deltaT;

		angular_ctrl_params_buffer.get(params);

		//PRINTF("kp: %f, error: %f, dif: %f, sum: %f", params.kp, ctrl_error, dif, sum);

		float rps_rw = (params.kp * ctrl_error - params.kd * dif + params.ki * sum);

		//PRINTF("commanded RPS: %f\n", rps_rw);

		rw_cmd_vel.put(rps_rw);

		lastError = ctrl_error;
	}
};


#endif /* CONTROL_ANGULARRATECONTROL_H_ */
