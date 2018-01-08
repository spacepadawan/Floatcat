/*
 * cmd_ids.h
 *
 *  Created on: 14.12.2017
 *      Author: Hermann
 */

#ifndef COMM_CMD_IDS_H_
#define COMM_CMD_IDS_H_

/*
enum cmd_id {
	CMD_SET_RW_SPEED = 0,
	CMD_HELLO,
	CMD_SET_CAM_ANGLE
};
*/

enum cmd_id {
	CMD_SET_RW_SPEED = 'R',
	CMD_HELLO = 'H',
	CMD_SET_CAM_ANGLE = 'c',

	CMD_SET_SERVO1 = 'A',
	CMD_SET_SERVO2 = 'B',
	CMD_SET_SERVO3 = 'C',
	CMD_SET_SERVO4 = 'D',

	CMD_SET_RW_P = 'Q',
	CMD_SET_RW_I = 'W',
	CMD_SET_RW_D = 'E',

	CMD_SET_THRUSTER1 = 'j',
	CMD_SET_THRUSTER2 = 'k',
	CMD_SET_THRUSTER3 = 'l'
};



#endif /* COMM_CMD_IDS_H_ */
