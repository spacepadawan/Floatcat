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
	HYBRID,
	PWM2
};

struct CustomVector {
	float x, y;

	float dotProduct(CustomVector other) {
		return x * other.x + y * other.y;
	}

	float operator*(CustomVector other) { return dotProduct(other); }
} tv1 = {0.86, 0.5}, tv2 = {-0.86, 0.5}, tv3 = {-1, 0};

void setActuators(float tx, float ty, float m, ActuatorMode mode) {




	ThrusterPower tp;
	float angularVel;

	switch (mode)
	{
	case PWM2:
	{
		CustomVector t = {tx, ty};

		tp.t1 = t * tv1, tp.t2 = t * tv2, tp.t3 = t * tv3;

		break;
	}
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

	desiredRotationSpeedBuffer.put(m);
	thrusterPower.put(tp);

	//PRINTF("thrusters: %f, %f, %f\n", tp.t1, tp.t2, tp.t3);

}



class BasicPID {

private:
	//Pose sum = {0};
	Pose old;

	float sum_x, sum_y, sum_phi;


	ControlDebug debug;

public:

	void init() {
		PID_ControlParameters lat_params, heading_params;
		lat_params.kp = 1;
		lat_params.kd = 0;
		lat_params.ki = 0;
		lateral_ctrl_params.publish(lat_params);

		heading_params.kp = 1;
		heading_params.kd = 0;
		heading_params.ki = 0;
		heading_ctrl_params.publish(heading_params);
	}

	void loop(int64_t dt, const Pose &pose, const Pose &poseDot, const Pose &desired,
			PID_ControlParameters lat_params, PID_ControlParameters heading_params) {
		float deltaT = dt / SECONDS;

		//PRINTF("pose: %f, %f, %f\n", pose.x, pose.y, pose.phi);
		//PRINTF("desired: %f, %f, %f\n", desired.x, desired.y, desired.phi);

		Pose error = desired - pose;
		//PRINTF("error: %f, %f, %f ", error.x, error.y, error.phi);
		error = error.rotate(pose.phi);

		if (error.phi > 180) {
			error.phi -= 360;
		} else if (error.phi < -180) {
			error.phi += 360;
		}



		//PRINTF("error local: %f, %f, %f ", error.x, error.y, error.phi);

		debug.e_x = error.x;
		debug.e_y = error.y;
		debug.e_phi = error.phi;

		sum_x += error.x * deltaT;
		sum_y += error.y * deltaT;
		sum_phi += error.phi * deltaT;

		if ((pose.x > 0 && old.x < 0) || (pose.x < 0 && old.x > 0)) {
			sum_x = 0;
		}

		if ((pose.y > 0 && old.y < 0) || (pose.y < 0 && old.y > 0)) {
			sum_y = 0;
		}

		float tx = lat_params.kp * error.x - lat_params.kd * poseDot.x + lat_params.ki * sum_x;
		float ty = lat_params.kp * error.y - lat_params.kd * poseDot.y + lat_params.ki * sum_y;

		float m = heading_params.kp * error.phi - heading_params.kd * poseDot.phi + heading_params.ki * sum_phi;

		/*
		float distance = sqrtf(error.x * error.x + error.y * error.y);

		float vel = distance * 0.1;
		if (vel > desiredVelocity) {
			vel = desiredVelocity;
		}

		float vel_x = vel * error.x / distance;
		float vel_y = vel * error.y / distance;
		tx = (vel_x - poseDot.x);
		ty = (vel_y - poseDot.y);
	*/

		//PRINTF("control output1: %f, %f, %f\n", tx, ty, m);

		float length = sqrtf(tx * tx + ty * ty);

		tx /= length;
		ty /= length;

		debug.tx = tx;
		debug.ty = ty;
		debug.m = m;

		controlDebugBuffer.put(debug);

		//PRINTF("control output2: %f, %f, %f\n", tx, ty, m);

		setActuators(tx, ty, m, PWM);
		old = pose;
	}


};



#endif /* CONTROL_BASIC_PID_H_ */
