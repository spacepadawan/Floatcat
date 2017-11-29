/*
 * topics.cpp
 *
 *  Created on: 28.11.2017
 *      Author: Hermann
 */

#include "topics.h"

Topic<SensorData> SensorDataTopic(-1, "Sensor Data");
CommBuffer<SensorData> SensorDataBuffer;
Subscriber SensorDataSubscriber(SensorDataTopic, SensorDataBuffer);
