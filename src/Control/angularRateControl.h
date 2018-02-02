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
		float deltaT = dt / SECONDS;
		float desired = 0;
		desiredRotationSpeedBuffer.get(desired);

		Pose poseDot;
		poseDotBuffer.get(poseDot);

		//angularRateBuffer.get(angularRate);

		//PRINTF("angularRate: %f ", angularRate);

		float error =  poseDot.phi - desired;

		sum += error * deltaT;

		if ((error >= 0 && lastError <= 0) || (error <= 0 && lastError >= 0)) {
			sum = 0;
		}

		dif = (error - lastError) / deltaT;

		angular_ctrl_params_buffer.get(params);

		//PRINTF("kp: %f ", params.kp);

		float rps_rw = (params.kp * error * error * error - params.kd * dif + params.ki * sum);

		//PRINTF("commanded RPS: %f\n", rps_rw);

		rw_cmd_vel.put(rps_rw);

		lastError = error;
	}
};


#endif /* CONTROL_ANGULARRATECONTROL_H_ */
