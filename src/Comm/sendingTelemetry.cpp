/*
 * sendingTelemetry.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: root
 */
#include<stdio.h>
#include"rodos.h"
#include "math.h"
#include "wifi_topics.h"

#include "../topics.h"

Telemetry tm;

class TelemetryData :public Thread{

public:


	void run(void)
	{
		init();

		SensorData sd;
		PowerValues p_values;
		Pose pose;
		RangeData rd;

		TIME_LOOP(NOW(), 500 * MILLISECONDS)
		{
			//PRINTF("Sending TM\n");

			//TODO catch all the tm and forward it
			SensorDataInfo.get(sd);

			tm.ax = sd.accelerometer.x;
			tm.ay = sd.accelerometer.y;
			tm.az = sd.accelerometer.z;

			tm.gx = sd.gyroscope.x;
			tm.gy = sd.gyroscope.y;
			tm.gz = sd.gyroscope.z;

			tm.mx = sd.magnetometer.x;
			tm.my = sd.magnetometer.y;
			tm.mz = sd.magnetometer.z;



			poseInfo.get(pose);
			tm.phi = pose.phi;
			// Example:
			tm.x = pose.x;
			tm.y = pose.y;

			float rps;
			rw_rpsInfo.get(rps);
			tm.enc_rps = rps;

			powerValuesInfo.get(p_values);

			tm.i_rw = p_values.i_rw;
			tm.v_bat = p_values.v_batt;

			rangeDataInfo.get(rd);

			tm.r_left = rd.left;
			tm.r_right = rd.right;

			starTrackerDataInfo.get(pose);
			tm.star_x = pose.x;
			tm.star_y = pose.y;
			tm.star_phi = pose.phi;

			poseDotInfo.get(pose);
			tm.x_dot = pose.x;
			tm.y_dot = pose.y;
			tm.phi_dot = pose.phi;

			//PRINTF("left: %f, right: %f \n", tm.rangeLeft, tm.rangeRight);

			telemetry.publish(tm);
		} // end time loop
	} // run ends

} TelemetrySender;



