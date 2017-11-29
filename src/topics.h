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

extern Topic<SensorData> SensorDataTopic;
extern CommBuffer<SensorData> SensorDataBuffer;
extern Subscriber SensorDataSubscriber;

#endif /* TOPICS_H_ */
