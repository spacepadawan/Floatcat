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

		TIME_LOOP(NOW(), 1000 * MILLISECONDS)
		{
			//PRINTF("Sending TM\n");

			//TODO catch all the tm and forward it
			SensorDataBuffer.get(sd);

			tm.ax = sd.accelerometer.x;
			tm.ay = sd.accelerometer.y;
			tm.az = sd.accelerometer.z;

			tm.gx = sd.gyroscope.x;
			tm.gy = sd.gyroscope.y;
			tm.gz = sd.gyroscope.z;

			tm.mx = sd.magnetometer.x;
			tm.my = sd.magnetometer.y;
			tm.mz = sd.magnetometer.z;



			poseBuffer.get(pose);
			tm.phi = pose.phi;
			// Example:
			tm.x = 42;
			tm.y = 17;

			double rps;
			rw_rps.get(rps);
			tm.enc_rps = rps;

			powerValues.get(p_values);

			tm.i_rw = p_values.i_rw;
			tm.v_bat = p_values.v_batt;

			telemetry.publish(tm);
		} // end time loop
	} // run ends

} TelemetrySender;



