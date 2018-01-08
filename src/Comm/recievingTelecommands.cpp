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
		PID_ControlParameters rwCtrlParams;
		ThrusterPower tpBuf;

		Telecommand* command = (Telecommand*) &data;
		recievedCommandNumber = command->data;
		recievedCommand = command->id;

		switch (command->id) {
		case CMD_SET_RW_SPEED:
			rw_cmd_power.put(command->data);
			PRINTF("RW speed desired value: %f\n", command->data);
			break;
		case CMD_HELLO:
			PRINTF("Hello\n");
			break;
		case CMD_SET_CAM_ANGLE:
			PRINTF("Cam angle not implemented yet\n");
			break;
		case CMD_SET_SERVO1:
			servoPositions.get(pos);
			pos.pos1 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo1 position: %f\n", command->data);
			break;
		case CMD_SET_SERVO2:
			servoPositions.get(pos);
			pos.pos2 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo2 position: %f\n", command->data);
			break;
		case CMD_SET_SERVO3:
			servoPositions.get(pos);
			pos.pos3 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo3 position: %f\n", command->data);
			break;
		case CMD_SET_SERVO4:
			servoPositions.get(pos);
			pos.pos4 = command->data;
			servoPositions.put(pos);
			PRINTF("Servo4 position: %f\n", command->data);
			break;
		case CMD_SET_RW_P:
			rw_ctrl_params.get(rwCtrlParams);
			rwCtrlParams.kp = command->data;
			rw_ctrl_params.put(rwCtrlParams);
			PRINTF("kp set: %f\n", command->data);
			break;
		case CMD_SET_RW_I:
			rw_ctrl_params.get(rwCtrlParams);
			rwCtrlParams.ki = command->data;
			rw_ctrl_params.put(rwCtrlParams);
			PRINTF("ki set: %f\n", command->data);
			break;
		case CMD_SET_RW_D:
			rw_ctrl_params.get(rwCtrlParams);
			rwCtrlParams.kd = command->data;
			rw_ctrl_params.put(rwCtrlParams);
			PRINTF("kd set: %f\n", command->data);
			break;
		case CMD_SET_THRUSTER1:
			thrusterPower.get(tpBuf);
			tpBuf.t1 = command->data;
			thrusterPower.put(tpBuf);
			break;
		case CMD_SET_THRUSTER2:
			thrusterPower.get(tpBuf);
			tpBuf.t2 = command->data;
			thrusterPower.put(tpBuf);
			break;
		case CMD_SET_THRUSTER3:
			thrusterPower.get(tpBuf);
			tpBuf.t3 = command->data;
			thrusterPower.put(tpBuf);
			break;
		default:
			PRINTF("unknown command\n");
		}

	}  // end of put
} MyTelecommand;
