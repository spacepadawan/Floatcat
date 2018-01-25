/*
 * Topics.h
 *
 *  Created on: 06.11.2017
 *      Author: Hermann
 */

#ifndef TOPICS_H_
#define TOPICS_H_

#include "matlib.h"

struct SensorData{
	RODOS::Vector3D_F accelerometer;
	RODOS::Vector3D_F gyroscope;
	RODOS::Vector3D_F magnetometer;
};

struct ServoData {
	float pos1, pos2, pos3, pos4;
};

struct PID_ControlParameters {
	float kp, ki, kd;
};

struct PowerValues {
	float i_batt, v_batt, i_rw;
};

union ThrusterPower {
	struct {
		float t1, t2, t3;
	};
	float arr[3];
};

struct Pose {
	float x, y, phi;
};

extern bool dcdcOn;

//extern CommBuffer<double> rw_rps;

extern Topic<SensorData> SensorDataTopic;
extern CommBuffer<SensorData> SensorDataBuffer;
extern Subscriber SensorDataSubscriber;

extern CommBuffer<double> rw_rps;
extern CommBuffer<float> rw_cmd_power;
extern CommBuffer<ServoData> servoPositions;
extern CommBuffer<PID_ControlParameters> rw_ctrl_params;
extern CommBuffer<PowerValues> powerValues;
extern CommBuffer<ThrusterPower> thrusterPower;

extern CommBuffer<Pose> poseBuffer;
extern CommBuffer<Pose> starTrackerDataBuffer;

#endif /* TOPICS_H_ */
