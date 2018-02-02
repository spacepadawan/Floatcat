/*
 * basic_pid.h
 *
 *  Created on: 29.01.2018
 *      Author: Hermann
 */

#ifndef CONTROL_BASIC_PID_H_
#define CONTROL_BASIC_PID_H_


#include "rodos.h"
#include "matlib.h"
#include "../topics.h"

enum ActuatorMode {
	PWM = 0,
	SERVO,
	HYBRID
};

void setActuators(float tx, float ty, float m, ActuatorMode mode) {


	ThrusterPower tp;
	float angularVel;

	switch (mode)
	{
	case PWM:
		if (tx > 0) {
			tp.t1 += 0.43301 * tx;
			tp.t3 += 0.25 * tx;
		} else {
			tp.t2 += - 0.43301 * tx;
			tp.t3 += - 0.25 * tx;
		}


		if (ty > 0) {
			tp.t1 += 0.25 * ty;
			tp.t2 += 0.25 * ty;
		} else {
			tp.t3 += - 0.5 * ty;
		}


		//if (m > 1.5 || m < -1.5)
		//angularVel = m;
		//desiredRotationSpeedBuffer.put(angularVel);
		break;
	default:
		return;
	}

	//rw_cmd_vel.put(rw);
	thrusterPower.put(tp);

	//PRINTF("thrusters: %f, %f, %f\n", tp.t1, tp.t2, tp.t3);

}



class BasicPID {

private:
	Pose sum = {0};
	Pose old;

	PID_ControlParameters params;

public:

	void init() {
		params.kp = 1;
		params.kd = 0;
		params.ki = 0;
		lateral_ctrl_params.publish(params);
	}

	void loop(int64_t dt) {
		float deltaT = dt / SECONDS;

		Pose pose;
		poseBuffer.get(pose);
		//Pose local = globalToLocal(global);

		Pose desired;
		desiredPoseBuffer.get(desired);

		//PRINTF("pose: %f, %f, %f\n", pose.x, pose.y, pose.phi);
		//PRINTF("desired: %f, %f, %f\n", desired.x, desired.y, desired.phi);

		Pose error = desired - pose;
		//PRINTF("error: %f, %f, %f ", error.x, error.y, error.phi);
		error = error.rotate(pose.phi);

		//PRINTF("error local: %f, %f, %f ", error.x, error.y, error.phi);

		sum = sum + error * deltaT;

		lateral_ctrl_params_buffer.get(params);


		float kx = params.kp, ky = params.kp, k_phi = 1;

		float tx = kx * error.x;
		float ty = ky * error.y;
		float m = k_phi * error.phi;

		//PRINTF("control output1: %f, %f, %f\n", tx, ty, m);

		tx = tx > 1 ? 1 : tx;
		ty = ty > 1 ? 1 : ty;
		tx = tx < -1 ? -1 : tx;
		ty = ty < -1 ? -1 : ty;

		//PRINTF("control output2: %f, %f, %f\n", tx, ty, m);

		setActuators(tx, ty, m, PWM);
		old = pose;
	}


};



#endif /* CONTROL_BASIC_PID_H_ */
