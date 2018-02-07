/*
 * vp_cont.h
 *
 *  Created on: 07.02.2018
 *      Author: Hermann
 */

#ifndef CONTROL_VP_CONT_H_
#define CONTROL_VP_CONT_H_

//#define CONTROL_DEBUG

#include "rodos.h"
#include "../topics.h"

class VPCont {

	float sum_phi = 0;

	ControlDebug debug;

public:

	void init() {
		PID_ControlParameters lat_params, heading_params;
		lat_params.kp = 0.05;
		lat_params.kd = 2;
		lat_params.ki = 0;
		lateral_ctrl_params.publish(lat_params);

		heading_params.kp = 1;
		heading_params.kd = 0;
		heading_params.ki = 0;
		heading_ctrl_params.publish(heading_params);
	}

	void loop(int64_t dt, const Pose &pose, const Pose &poseDot, const Pose &desired,
			PID_ControlParameters lat_params, PID_ControlParameters heading_params)
	{
		float deltaT = (float) dt / (float) SECONDS;

		Pose errorPose = desired - pose;
		errorPose = errorPose.rotate(pose.phi);

		Pose poseDotLocal = poseDot.rotate(pose.phi);

		if (errorPose.phi >= 180) {
			errorPose.phi -= 360;
		} else if (errorPose.phi < -180) {
			errorPose.phi += 360;
		}

		debug.e_x = errorPose.x;
		debug.e_y = errorPose.y;
		debug.e_phi = errorPose.phi;

		//sum_phi += errorPose.phi * deltaT;

		float m = heading_params.kp * errorPose.phi - heading_params.kd * poseDot.phi + heading_params.ki * sum_phi;

#ifndef CONTROL_DEBUG
		desiredRotationSpeedBuffer.put(m);
#endif

		float distance = sqrtf(errorPose.x * errorPose.x + errorPose.y * errorPose.y);

		float gain = distance * lat_params.kd;
		gain = gain > 1 ? 1 : gain;

		float vx_des = errorPose.x * gain * lat_params.kp / distance;
		float vy_des = errorPose.y * gain * lat_params.kp / distance;

		float vx_e = vx_des - poseDotLocal.x;
		float vy_e = vy_des - poseDotLocal.y;

		debug.tx = poseDotLocal.x;
		debug.ty = poseDotLocal.y;

		float alpha = atan2f(vx_e, vy_e);

		if (alpha <= - M_PI) {
			alpha += 2 * M_PI;
		} else if (alpha > M_PI) {
			alpha -= 2 * M_PI;
		}

		debug.m = alpha;

		ThrusterPower tp;
		float gamma = 0;

		if (alpha < 1.0472 && alpha >= -1.0472) {
			gamma = (alpha + 1.0472) / 2.0944;
			tp.t3 = 0;
			tp.t1 = gamma;
			tp.t2 = 1 - gamma;
		} else if (alpha >= 1.0472) {
			gamma = (alpha - 1.0472) / 2.0944;
			tp.t2 = 0;
			tp.t3 = gamma;
			tp.t1 = 1 - gamma;
		} else {
			gamma = (alpha + 3.1416) / 2.0944;
			tp.t1 = 0;
			tp.t2 = gamma;
			tp.t3 = 1 - gamma;
		}

		float strength = sqrtf((0.86 * tp.t1 - 0.86 * tp.t2) * (0.86 * tp.t1 - 0.86 * tp.t2)
				+ (0.5 * tp.t1 + 0.5 * tp.t2 - tp.t3) * (0.5 * tp.t1 + 0.5 * tp.t2 - tp.t3));

		tp.t1 *= gain * 0.4 / strength;
		tp.t2 *= gain * 0.4 / strength;
		tp.t3 *= gain * 0.4 / strength;

#ifndef CONTROL_DEBUG
		thrusterPower.put(tp);
#endif

		debug.t1 = tp.t1;
		debug.t2 = tp.t2;
		debug.t3 = tp.t3;

		controlDebugBuffer.put(debug);

	}
};


#endif /* CONTROL_VP_CONT_H_ */
