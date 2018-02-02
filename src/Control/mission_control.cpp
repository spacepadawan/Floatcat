/*
 * mission_control.cpp
 *
 *  Created on: 31.01.2018
 *      Author: Hermann
 */

#include "rodos.h"
#include "../topics.h"

enum ControlStage {
	SEARCHING_VEHICLE,
	APPROACHING,
	DOCKING
};


class MissionControl : public Thread {
	OperationMode mode;
	ControlStage stage;
public:
	MissionControl(const char* name) : Thread(name) {

	}

	void init() {

	}

	void run() {
		TIME_LOOP(NOW() + 5 * SECONDS, 100 * MILLISECONDS) {
			operationModeBuffer.get(mode);
			if (mode == OP_MISSION) {

			}
		}
	}


};

MissionControl mCnrtl("MissionControl");

