/*
 * Topics.h
 *
 *  Created on: 06.11.2017
 *      Author: Hermann
 */

#ifndef TOPICS_H_
#define TOPICS_H_

#include "matlib.h"
#include "Misc/pose.h"

struct PID_ControlParameters {
	float kp, ki, kd;
};

struct ControlDebug {
	float e_x, e_y, e_phi, tx, ty, m, t1, t2, t3;
};

struct CalibrationData {
	float gz, ax, ay;
};

struct RadioData {
	float x1, y1, x2, y2;
};

enum OperationMode {
	OP_MANUAL,
	OP_ANGULAR_RATE_CONTROL,
	OP_HEADING_CONTROL,
	OP_POSE_CONTROL,
	OP_MISSION
};

enum CameraMode {
	STARTRACKER,
	OBJECTDETECTION
};

enum RaspberryCommand {
	CMD_START_STARTRACKER,
	CMD_START_OBJECTDETECTION,
	CMD_START_RADIO,
	CMD_PAUSE
};

struct SensorData{
	RODOS::Vector3D_F accelerometer;
	RODOS::Vector3D_F gyroscope;
	RODOS::Vector3D_F magnetometer;
};

struct ServoData {
	float pos1, pos2, pos3, pos4;
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

struct RangeData {
	uint8_t left, right;
};

extern CommBuffer<ControlDebug> controlDebugBuffer;

extern Topic<SensorData> SensorDataTopic;

extern Topic<float> rw_rpsTopic;


extern CommBuffer<float> rw_cmd_vel;
extern CommBuffer<ServoData> servoPositions;

// the power sensors distribute there measurements here
extern Topic<PowerValues> powerValuesTopic;

// the pwm value for the thrusters gets distributed to the thruster threads here
extern CommBuffer<ThrusterPower> thrusterPower;

// this is fed to the telemetry directly and its being used in control as true pose
extern Topic<Pose> poseTopic;

// here the pose measured by the star tracker gets distributed
extern Topic<Pose> starTrackerPoseTopic;

extern CommBuffer<Pose> desiredPoseBuffer;
extern Topic<RangeData> rangeSensorTopic;

extern CommBuffer<OperationMode> operationModeBuffer;

extern CommBuffer<float> desiredRotationSpeedBuffer;

extern CommBuffer<CameraMode> cameraModeBuffer;
extern CommBuffer<RaspberryCommand> raspberryCMDBuffer;

extern Topic<Pose> poseDotTopic;

extern Topic<CalibrationData> calibTopic;
extern Topic<ServoData> servoCalibrationTopic;
extern Topic<bool> dcdcOn;

extern Topic<PID_ControlParameters> rw_ctrl_params;
extern Topic<PID_ControlParameters> angular_ctrl_params;
extern Topic<PID_ControlParameters> lateral_ctrl_params;
extern Topic<PID_ControlParameters> heading_ctrl_params;


#endif /* TOPICS_H_ */
