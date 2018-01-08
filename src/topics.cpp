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

CommBuffer<double> rw_rps;
CommBuffer<float> rw_cmd_power;
CommBuffer<ServoData> servoPositions;
CommBuffer<PID_ControlParameters> rw_ctrl_params;
CommBuffer<PowerValues> powerValues;
CommBuffer<ThrusterPower> thrusterPower;

CommBuffer<Pose> poseBuffer;

bool dcdcOn = false;
