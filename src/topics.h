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

Topic<SensorData> SensorDataTopic(-1, "Sensor Data");
CommBuffer<SensorData> SensorDataBuffer;
Subscriber SensorDataSubscriber(SensorDataTopic, SensorDataBuffer);

#endif /* TOPICS_H_ */
