/*
 * topics.cpp
 *
 *  Created on: 28.11.2017
 *      Author: Hermann
 */

#include "topics.h"

Topic<SensorData> SensorDataTopic(28, "Sensor Data");
CommBuffer<SensorData> SensorDataBuffer;
CommBuffer<SensorData> SensorDataInfo;
Subscriber SensorDataSub1(SensorDataTopic, SensorDataBuffer);
Subscriber SensorDataSub2(SensorDataTopic, SensorDataInfo);

CommBuffer<float> rw_rpsBuffer;
CommBuffer<float> rw_rpsInfo;
Topic<float> rw_rpsTopic(19, "rw_rps");
Subscriber rw_rpsSub1(rw_rpsTopic, rw_rpsBuffer);
Subscriber rw_rpsSub2(rw_rpsTopic, rw_rpsInfo);

CommBuffer<float> rw_cmd_vel;
CommBuffer<ServoData> servoPositions;

CommBuffer<PowerValues> powerValuesBuffer;
CommBuffer<PowerValues> powerValuesInfo;
Topic<PowerValues> powerValuesTopic(18, "Power Values");
Subscriber powerValuesSub1(powerValuesTopic, powerValuesBuffer);
Subscriber powerValuesSub2(powerValuesTopic, powerValuesInfo);

CommBuffer<ThrusterPower> thrusterPower;

CommBuffer<Pose> poseBuffer;
CommBuffer<Pose> poseInfo;
Topic<Pose> poseTopic(13, "PoseData");
Subscriber poseSubscriber1(poseTopic, poseBuffer);
Subscriber poseSubscriber2(poseTopic, poseInfo);

CommBuffer<Pose> starTrackerDataBuffer;
CommBuffer<Pose> starTrackerDataInfo;
Topic<Pose> starTrackerPoseTopic(25, "Star Tracker Data");
Subscriber starTrackerPoseSubscriber(starTrackerPoseTopic, starTrackerDataBuffer);
Subscriber starTrackerPoseSubscriber2(starTrackerPoseTopic, starTrackerDataInfo);

CommBuffer<Pose> desiredPoseBuffer;
CommBuffer<RangeData> rangeDataBuffer;
CommBuffer<RangeData> rangeDataInfo;

Topic<RangeData> rangeSensorTopic(420, "RangeSensorData");
Subscriber rangeSensorSub1(rangeSensorTopic, rangeDataBuffer);
Subscriber rangeSensorSub2(rangeSensorTopic, rangeDataInfo);

CommBuffer<OperationMode> operationModeBuffer;

CommBuffer<float> desiredRotationSpeedBuffer;

CommBuffer<CameraMode> cameraModeBuffer;
CommBuffer<RaspberryCommand> raspberryCMDBuffer;

//CommBuffer<float> angularRateBuffer;
//CommBuffer<float> angularRateInfo;
//Topic<float> angularRateTopic(17, "AngularRate");

CommBuffer<Pose> poseDotBuffer;
CommBuffer<Pose> poseDotInfo;
Topic<Pose> poseDotTopic(17, "Pose change");
Subscriber poseDotSub1(poseDotTopic, poseDotBuffer);
Subscriber poseDotSub2(poseDotTopic, poseDotInfo);

Topic<CalibrationData> calibTopic(421, "imuCalib");
Topic<ServoData> servoCalibrationTopic(422, "ServoCalib");
Topic<bool> dcdcOn(-1, "DCDC");

Topic<PID_ControlParameters> rw_ctrl_params(432, "rwParams");
Topic<PID_ControlParameters> angular_ctrl_params(433, "angParams");
Topic<PID_ControlParameters> lateral_ctrl_params(434, "latParams");
Topic<PID_ControlParameters> heading_ctrl_params(435, "headingParams");

CommBuffer<CalibrationData> imuCalibrationBuffer;
Subscriber imuCalibSub1(calibTopic, imuCalibrationBuffer);

CommBuffer<bool> dcdcOnBuffer;
Subscriber dcdcSub1(dcdcOn, dcdcOnBuffer);

CommBuffer<ServoData> servoCalibrationBuffer;
Subscriber servoDataSub1(servoCalibrationTopic, servoCalibrationBuffer);

CommBuffer<PID_ControlParameters> rw_ctrl_params_buffer;
CommBuffer<PID_ControlParameters> angular_ctrl_params_buffer;
CommBuffer<PID_ControlParameters> lateral_ctrl_params_buffer;
CommBuffer<PID_ControlParameters> heading_ctrl_params_buffer;
Subscriber rw_ctrl_sub1(rw_ctrl_params, rw_ctrl_params_buffer);
Subscriber angular_ctrl_sub1(angular_ctrl_params, angular_ctrl_params_buffer);
Subscriber lateral_ctrl_sub1(lateral_ctrl_params, lateral_ctrl_params_buffer);
Subscriber heading_ctr1_sub1(heading_ctrl_params, heading_ctrl_params_buffer);

CommBuffer<CalibrationData> imuCalibrationInfo;
Subscriber imuCalibSub2(calibTopic, imuCalibrationInfo);

CommBuffer<bool> dcdcOnInfo;
Subscriber dcdcSub2(dcdcOn, dcdcOnInfo);

CommBuffer<ServoData> servoCalibrationInfo;
Subscriber servoDataSub2(servoCalibrationTopic, servoCalibrationInfo);

CommBuffer<PID_ControlParameters> rw_ctrl_params_info;
CommBuffer<PID_ControlParameters> angular_ctrl_params_info;
CommBuffer<PID_ControlParameters> lateral_ctrl_params_info;
CommBuffer<PID_ControlParameters> heading_ctrl_params_info;
Subscriber rw_ctrl_sub2(rw_ctrl_params, rw_ctrl_params_info);
Subscriber angular_ctr2_sub1(angular_ctrl_params, angular_ctrl_params_info);
Subscriber lateral_ctr2_sub1(lateral_ctrl_params, lateral_ctrl_params_info);
Subscriber heading_ctr2_sub1(heading_ctrl_params, heading_ctrl_params_info);
