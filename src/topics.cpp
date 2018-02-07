/*
 * topics.cpp
 *
 *  Created on: 28.11.2017
 *      Author: Hermann
 */

#include "topics.h"

CommBuffer<ControlDebug> controlDebugBuffer;

Topic<SensorData> SensorDataTopic(28, "Sensor Data");

CommBuffer<SensorData> SensorDataBuffer;
Subscriber SensorDataSub1(SensorDataTopic, SensorDataBuffer);

Topic<float> rw_rpsTopic(19, "rw_rps");


CommBuffer<float> rw_cmd_vel;
CommBuffer<ServoData> servoPositions;

CommBuffer<PowerValues> powerValuesBuffer;

Topic<PowerValues> powerValuesTopic(18, "Power Values");
Subscriber powerValuesSub1(powerValuesTopic, powerValuesBuffer);

CommBuffer<ThrusterPower> thrusterPower;

Topic<Pose> poseTopic(13, "PoseData");

CommBuffer<Pose> starTrackerDataBuffer;

Topic<Pose> starTrackerPoseTopic(25, "Star Tracker Data");
Subscriber starTrackerPoseSubscriber(starTrackerPoseTopic, starTrackerDataBuffer);



CommBuffer<Pose> desiredPoseBuffer;


Topic<RangeData> rangeSensorTopic(420, "RangeSensorData");



CommBuffer<OperationMode> operationModeBuffer;
CommBuffer<float> desiredRotationSpeedBuffer;

CommBuffer<CameraMode> cameraModeBuffer;
CommBuffer<RaspberryCommand> raspberryCMDBuffer;

Topic<Pose> poseDotTopic(17, "Pose change");

//CommBuffer<float> angularRateBuffer;
//CommBuffer<float> angularRateInfo;
//Topic<float> angularRateTopic(17, "AngularRate");

Topic<CalibrationData> calibTopic(421, "imuCalib");
Topic<ServoData> servoCalibrationTopic(422, "ServoCalib");
Topic<bool> dcdcOn(-1, "DCDC");

Topic<PID_ControlParameters> rw_ctrl_params(432, "rwParams");
Topic<PID_ControlParameters> angular_ctrl_params(433, "angParams");
Topic<PID_ControlParameters> lateral_ctrl_params(434, "latParams");
Topic<PID_ControlParameters> heading_ctrl_params(435, "headingParams");
