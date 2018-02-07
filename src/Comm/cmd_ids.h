/*
 * cmd_ids.h
 *
 *  Created on: 14.12.2017
 *      Author: Hermann
 */

#ifndef COMM_CMD_IDS_H_
#define COMM_CMD_IDS_H_


enum cmd_id {
	CMD_SET_RW_SPEED = 'R',
	CMD_HELLO = 'H',
	CMD_SET_CAM_ANGLE = 'c',

	CMD_SET_SERVO1 = 'A',
	CMD_SET_SERVO2 = 'B',
	CMD_SET_SERVO3 = 'C',
	CMD_SET_SERVO4 = 'D',

	CMD_SEND_PARAMS = 'Q',

	CMD_SET_THRUSTER1 = 'j',
	CMD_SET_THRUSTER2 = 'k',
	CMD_SET_THRUSTER3 = 'l',

	CMD_SET_ROTATIONSPEED	= 'm',

	CMD_SET_MODE_MANUAL = 'y',
	CMD_SET_MODE_ANGULAR_CONTROL = 'a',
	CMD_SET_MODE_HEADING_CONTROL = 'h',
	CMD_SET_MODE_POSE_CONTROL = 'x',
	CMD_SET_MODE_MISSION = 'M'

};



#endif /* COMM_CMD_IDS_H_ */
