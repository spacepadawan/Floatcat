/*
 * controller.cpp
 *
 *  Created on: 30.01.2018
 *      Author: Hermann
 */


#include "rodos.h"
#include "basic_pid.h"
#include "vp_cont.h"
#include "heading_ctrl.h"
#include "../topics.h"

#include "angularRateControl.h"

CommBuffer<Pose> poseBuffer;
Subscriber poseSubscriber1(poseTopic, poseBuffer);

CommBuffer<PID_ControlParameters> angular_ctrl_params_buffer;
CommBuffer<PID_ControlParameters> lateral_ctrl_params_buffer;
CommBuffer<PID_ControlParameters> heading_ctrl_params_buffer;
Subscriber angular_ctrl_sub1(angular_ctrl_params, angular_ctrl_params_buffer);
Subscriber lateral_ctrl_sub1(lateral_ctrl_params, lateral_ctrl_params_buffer);
Subscriber heading_ctr1_sub1(heading_ctrl_params, heading_ctrl_params_buffer);

CommBuffer<Pose> poseDotBuffer;
Subscriber poseDotSub1(poseDotTopic, poseDotBuffer);

CommBuffer<float> rw_rpsBuffer;
Subscriber rw_rpsSub1(rw_rpsTopic, rw_rpsBuffer);


class ControlThread : public Thread
{
private:
	//BasicPID pid;
	VPCont cont;
	HeadingCtrl headingCtrl;

	RPMControl rpmCntrl;

	OperationMode mode;
	Pose pose;

	PID_ControlParameters ang_params, lat_params, heading_params;

public:
	ControlThread(const char* name) : Thread(name) { }

	void init() {
		mode = OP_MANUAL;
		operationModeBuffer.put(mode);
	}

	void run() {
		cont.init();
		rpmCntrl.init();

		int64_t dt = 20 * MILLISECONDS;

		Pose pose, desired, poseDot;
		float currentRPS;

		TIME_LOOP(NOW() + 10 * MILLISECONDS, dt) {
			operationModeBuffer.get(mode);
			poseBuffer.get(pose);
			desiredPoseBuffer.get(desired);
			poseDotBuffer.get(poseDot);

			angular_ctrl_params_buffer.get(ang_params);
			lateral_ctrl_params_buffer.get(lat_params);
			heading_ctrl_params_buffer.get(heading_params);

			rw_rpsBuffer.get(currentRPS);

			switch (mode) {
			case OP_ANGULAR_RATE_CONTROL:
				rpmCntrl.loop(dt, poseDot, currentRPS, ang_params);
				break;
			case OP_HEADING_CONTROL:
				headingCtrl.loop(dt, pose, poseDot, desired, heading_params);
				break;
			case OP_POSE_CONTROL:

				cont.loop(dt, pose, poseDot, desired, lat_params, heading_params);

#ifndef CONTROL_DEBUG
				rpmCntrl.loop(dt, poseDot, currentRPS, ang_params);
#endif
				break;
			}
		}
	}
};

ControlThread cntrl("ControlThread");
