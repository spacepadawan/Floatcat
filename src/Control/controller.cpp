/*
 * controller.cpp
 *
 *  Created on: 30.01.2018
 *      Author: Hermann
 */


#include "rodos.h"
#include "basic_pid.h"
#include "../topics.h"

#include "angularRateControl.h"

class ControlThread : public Thread
{
private:
	BasicPID pid;

	RPMControl rpmCntrl;

	OperationMode mode;
	Pose pose;

public:
	ControlThread(const char* name) : Thread(name) { }

	void init() {
		mode = OP_ANGULAR_RATE_CONTROL;
		operationModeBuffer.put(mode);
	}

	void run() {
		pid.init();
		rpmCntrl.init();

		int64_t dt = 20 * MILLISECONDS;

		TIME_LOOP(NOW() + 10 * MILLISECONDS, dt) {
			operationModeBuffer.get(mode);
			poseBuffer.get(pose);

			switch (mode) {
			case OP_ANGULAR_RATE_CONTROL:
				rpmCntrl.loop(dt);
				break;
			case OP_POSE_CONTROL:
				pid.loop(dt);
				//rpmCntrl.loop(dt);
				break;
			}
		}
	}
};

ControlThread cntrl("ControlThread");
