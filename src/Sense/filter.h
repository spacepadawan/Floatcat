/*
 * filter.h
 *
 *  Created on: 19.12.2017
 *      Author: Hermann
 */

#include "matlib.h"
#include "rodos.h"
#include "../topics.h"

#ifndef SENSE_FILTER_H_
#define SENSE_FILTER_H_

#define LOWPASS_SAMPLES		8


class Filter : SubscriberReceiver<Pose> {

private:
	float factor = 0; //0.05; // between 0 and 1, 0 means only gyro, 1 means only magnetometer

	Pose pose;
	Pose oldPose;

	Pose poseDot;

	int64_t updateTime;

	float angularRateSum = 0;
	float angularRates[LOWPASS_SAMPLES];
	uint8_t currentSumIndex = 0;

	float vx = 0, vy = 0;

public:
	Filter() : SubscriberReceiver<Pose>(starTrackerPoseTopic, "StarTrackerPoseReceived") {
		pose.x = 0;
		pose.y = 0;
		pose.phi = 0;
		oldPose = pose;
	}

	void init() {}

	void put(Pose &p) {
		updatePose(p);
	}

	Pose filterPose(SensorData* sd, uint64_t dt);
	void updatePose(Pose p);
};


#endif /* SENSE_FILTER_H_ */
