/*
 * Sense.h
 *
 *  Created on: 04.11.2017
 *      Author: Hermann
 */

#ifndef SENSE_SENSE_CPP_
#define SENSE_SENSE_CPP_

#include "rodos.h"
#include "..\topics.h"
#include "imu.h"
#include "filter.h"

/*
 * GPIO_021		PB05 	MOSI
 * GPIO_019		PB03	CLK
 * GPIO_020		PB04	MISO
 * GPIO_018		PB02	CS_G
 * GPIO_032		PC00	CS_XM
 * GPIO_055		PD07	IMU_EN		switch IMU on/off
 */

HAL_SPI spi(SPI_IDX1, GPIO_019, GPIO_021, GPIO_020);
HAL_GPIO csXM(GPIO_032);
HAL_GPIO csG(GPIO_018);
HAL_GPIO imuEN(GPIO_055);

Semaphore senseSemaphore;

#define SENSE_PERIOD	(10 * MILLISECONDS)

class Sense: public Thread {
private:

	IMU imu;
	Filter filter;

	CalibrationData calib;

public:

	Sense(const char* name) :
			Thread(name),
			imu(IMU(&spi, &csXM, &csG, &imuEN, &senseSemaphore)) {

	}

	void init() {
		setPeriodicBeat(0, SENSE_PERIOD);
		spi.reset();
		spi.init();

		Pose initPose;
		initPose.phi = 0;
		initPose.x = 1;
		initPose.y = 1;

		poseBuffer.put(initPose);

		imu.init();
	}

#define CALIBRATE

	void run() {

		SensorData sd;
		Pose pose;

#ifdef CALIBRATE
		int16_t calibrationCounter = 0, samples = 100;
		double gyroXSum = 0, gyroYSum = 0, gyroZSum = 0;
		double accXSum = 0, accYSum = 0;
#endif
		calib = {0};
		calibTopic.publish(calib);

		while (1) {

			imu.readIMU(sd.accelerometer, sd.gyroscope, sd.magnetometer);


//#ifdef CALIBRATE
			if (calibrationCounter < samples) {
				//PRINTF("Calibration of gyro: %f, %f, %f\n", gyroXSum, gyroYSum, gyroZSum);
				//PRINTF("Calibration of acc: %f, %f\n", accXSum, accYSum);

				gyroZSum += sd.gyroscope.z;

				accXSum += sd.accelerometer.x;
				accYSum += sd.accelerometer.y;

				calibrationCounter++;
			} else if (calibrationCounter == samples) {
				accXSum /= samples;
				accYSum /= samples;
				gyroZSum /= samples;

				calib.gz = gyroZSum;
				calib.ax = accXSum;
				calib.ay = accYSum;
				calibTopic.publish(calib);

				calibrationCounter++;
			} else {
				imuCalibrationBuffer.getOnlyIfNewData(calib);

				sd.accelerometer.x -= calib.ax;
				sd.accelerometer.y -= calib.ay;
				sd.gyroscope.z -= calib.gz;
				SensorDataTopic.publish(sd, false);
				Pose pose = filter.filterPose(&sd, SENSE_PERIOD);
				poseTopic.publish(pose);
			}


//#endif


			//poseBuffer.get(pose);



			suspendUntilNextBeat();
		}
	}
};
Sense Sense("Sense");


#endif /* SENSE_SENSE_CPP_ */
