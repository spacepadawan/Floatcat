/*
 * recievingTelecommands.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: root
 */

#include <stdio.h>
#include "rodos.h"
#include "math.h"
#include "wifi_topics.h"
#include "cmd_ids.h"
#include "../topics.h"

class ParameterChangeHandler : public SubscriberReceiver<Parameters> {

public:
	ParameterChangeHandler() : SubscriberReceiver<Parameters>(parameterTopicIn, "ParameterReceived") {

	}

	void init() {

	}

	void put(Parameters &data) {
		Parameters* params = (Parameters*) &data;

		PID_ControlParameters ang_params, lat_params, rw_params, heading_params;

		ang_params.kp = params->ang_kp;
		ang_params.ki = params->ang_ki;
		ang_params.kd = params->ang_kd;

		lat_params.kp = params->pos_kp;
		lat_params.ki = params->pos_ki;
		lat_params.kd = params->pos_kd;

		rw_params.kp = params->rw_kp;
		rw_params.ki = params->rw_ki;
		rw_params.kd = params->rw_kd;

		heading_params.kp = params->heading_kp;
		heading_params.ki = params->heading_ki;
		heading_params.kd = params->heading_kd;

		angular_ctrl_params.publish(ang_params);
		lateral_ctrl_params.publish(lat_params);
		rw_ctrl_params.publish(rw_params);
		heading_ctrl_params.publish(heading_params);

		bool on = params->dcdc_on > 0.5;
		dcdcOn.publish(on);

		CalibrationData calib;

		calib.gz = params->gz;
		calib.ax = params->ax;
		calib.ay = params->ay;

		calibTopic.publish(calib);

		ServoData offsets;

		offsets.pos1 = params->off1;
		offsets.pos2 = params->off2;
		offsets.pos3 = params->off3;
		offsets.pos4 = params->off4;

		servoCalibrationTopic.publish(offsets);

		//PRINTF("%f, %d, %f, %f, %f, %f\n", params->pos_kp, params->dcdc_on, params->off1, params->off2, params->off3, params->off4);

	}
} pch;

class ControlCommandHandler : public SubscriberReceiver<ControlCommand> {
	ServoData pos;
	ThrusterPower tpBuf;
public :
	ControlCommandHandler() :
	SubscriberReceiver<ControlCommand>(controlCommandTopic, "ControlCommandReceived") {

	}

	void init() {

	}

	void put(ControlCommand &data) {

		ControlCommand* cmd = (ControlCommand*) &data;

		OperationMode mode;

		Pose desired;
		desired.x = cmd->x;
		desired.y = cmd->y;
		desired.phi = cmd->phi;
		desiredPoseBuffer.put(desired);

		PRINTF("desired Pose changed\n");

		switch (cmd->mode) {
		case MANUAL:
			mode = OP_MANUAL;
			//operationModeBuffer.put(mode);
			break;
		case POSITION_CONTROL:
			mode = OP_POSE_CONTROL;
			//operationModeBuffer.put(mode);


			break;
		case MISSION_CONTROL:
			mode = OP_MISSION;
			//operationModeBuffer.put(mode);
			break;
		default:

			break;
		}
	}
} controlCommandHandler;

class ThrusterCommandHandler : public SubscriberReceiver<ThrusterCommand> {
	ServoData pos;
	ThrusterPower tpBuf;
public :
	ThrusterCommandHandler() :
	SubscriberReceiver<ThrusterCommand>(thrusterCommandTopic, "ThrusterCommandReceived") {

	}

	void init() {

	}

	void put(ThrusterCommand &data) {

		ThrusterCommand* cmd = (ThrusterCommand*) &data;

		servoPositions.get(pos);

		pos.pos1 = cmd->servo1;
		pos.pos2 = cmd->servo2;
		pos.pos3 = cmd->servo3;

		tpBuf.t1 = cmd->thruster1;
		tpBuf.t2 = cmd->thruster2;
		tpBuf.t3 = cmd->thruster3;

		thrusterPower.put(tpBuf);
		servoPositions.put(pos);
	}
} thrusterCommandHandler;

class myTelecommand: public SubscriberReceiver<Telecommand> {
public:
	myTelecommand() :
			SubscriberReceiver<Telecommand>(telecommand, "TelecommandRecieved") {
	}
	char recievedCommand;
	float recievedCommandNumber;
	void init() {
	}
	void put(Telecommand &data) {

		ServoData pos;
		//PID_ControlParameters rwCtrlParams, angularCtrlParams, lateralCtrlParams;
		// dcdc_on;
		//CalibrationData calib;

		ThrusterPower tpBuf;
		Parameters params;

		Telecommand* command = (Telecommand*) &data;
		recievedCommandNumber = command->data;
		recievedCommand = command->id;

		OperationMode mode;
		operationModeBuffer.get(mode);

		switch (command->id) {
		case CMD_SET_RW_SPEED:
			if (mode != OP_MANUAL) break;

			rw_cmd_vel.put(command->data);
			PRINTF("RW speed desired value: %f\n", command->data);
			break;
		case CMD_HELLO:
			PRINTF("Hello\n");
			break;
		case CMD_SET_CAM_ANGLE:
			PRINTF("Cam angle not implemented yet\n");
			break;
		case CMD_SET_SERVO1:
			if (mode != OP_MANUAL) break;

			servoPositions.get(pos);
			pos.pos1 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo1 position: %f\n", command->data);
			break;
		case CMD_SET_SERVO2:
			if (mode != OP_MANUAL) break;

			servoPositions.get(pos);
			pos.pos2 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo2 position: %f\n", command->data);
			break;
		case CMD_SET_SERVO3:
			if (mode != OP_MANUAL) break;

			servoPositions.get(pos);
			pos.pos3 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo3 position: %f\n", command->data);
			break;
		case CMD_SET_SERVO4:
			if (mode != OP_MANUAL) break;

			servoPositions.get(pos);
			pos.pos4 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo4 position: %f\n", command->data);
			break;
		case CMD_SEND_PARAMS:

			PID_ControlParameters ang_params, lat_params, rw_params, heading_params;

			angular_ctrl_params_info.get(ang_params);
			lateral_ctrl_params_info.get(lat_params);
			rw_ctrl_params_info.get(rw_params);
			heading_ctrl_params_info.get(heading_params);

			params.ang_kp = ang_params.kp;
			params.ang_ki = ang_params.ki;
			params.ang_kd = ang_params.kd;

			params.pos_kp = lat_params.kp;
			params.pos_ki = lat_params.ki;
			params.pos_kd = lat_params.kd;

			params.rw_kp = rw_params.kp;
			params.rw_ki = rw_params.ki;
			params.rw_kd = rw_params.kd;

			params.heading_kp = heading_params.kp;
			params.heading_ki = heading_params.ki;
			params.heading_kd = heading_params.kd;

			bool on;
			dcdcOnInfo.get(on);
			params.dcdc_on = on ? 1 : 0;

			CalibrationData calib;
			imuCalibrationInfo.get(calib);

			params.gz = calib.gz;
			params.ax = calib.ax;
			params.ay = calib.ay;

			ServoData offsets;
			servoCalibrationInfo.get(offsets);

			params.off1 = offsets.pos1;
			params.off2 = offsets.pos2;
			params.off3 = offsets.pos3;
			params.off4 = offsets.pos4;

			//PRINTF("%f, %d, %f, %f, %f, %f\n", params.pos_kp, params.dcdc_on, params.off1, params.off2, params.off3, params.off4);

			parameterTopicOut.publish(params);

			break;
		case CMD_SET_THRUSTER1:
			if (mode != OP_MANUAL) break;

			thrusterPower.get(tpBuf);
			tpBuf.t1 = command->data;
			thrusterPower.put(tpBuf);
			break;
		case CMD_SET_THRUSTER2:
			if (mode != OP_MANUAL) break;

			thrusterPower.get(tpBuf);
			tpBuf.t2 = command->data;
			thrusterPower.put(tpBuf);
			break;
		case CMD_SET_THRUSTER3:
			if (mode != OP_MANUAL) break;

			thrusterPower.get(tpBuf);
			tpBuf.t3 = command->data;
			thrusterPower.put(tpBuf);
			break;
		case CMD_SET_ROTATIONSPEED:
			desiredRotationSpeedBuffer.put(command->data);
			break;
		case CMD_SET_MODE_MANUAL:
			mode = OP_MANUAL;
			operationModeBuffer.put(mode);
			break;
		case CMD_SET_MODE_ANGULAR_CONTROL:
			mode = OP_ANGULAR_RATE_CONTROL;
			operationModeBuffer.put(mode);
			break;
		case CMD_SET_MODE_POSE_CONTROL:
			mode = OP_POSE_CONTROL;
			operationModeBuffer.put(mode);
			break;
		case CMD_SET_MODE_MISSION:
			mode = OP_MISSION;
			operationModeBuffer.put(mode);
			break;
		default:
			PRINTF("unknown command\n");
		}



	}  // end of put
} MyTelecommand;
