/*
 * Sense.h
 *
 *  Created on: 04.11.2017
 *      Author: Hermann
 */

#ifndef SENSE_SENSE_H_
#define SENSE_SENSE_H_

#include "rodos.h"
#include "..\topics.h"
#include "imu.h"

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

class Sense: public Thread {
private:

	IMU imu;

public:

	Sense(const char* name) :
			Thread(name),
			imu(IMU(&spi, &csXM, &csG, &imuEN, &senseSemaphore)) {

	}

	void init() {
		setPeriodicBeat(0, 10 * MILLISECONDS);
		spi.reset();
		spi.init();

		imu.init();
	}

//#define CALIBRATE

	void run() {

		SensorData sd;
#ifdef CALIBRATE
		int16_t calibrationCounter = 0, samples = 1000;
		float gyroXSum = 0, gyroYSum = 0, gyroZSum = 0;
		float accXSum = 0, accYSum = 0;
#endif

		while (1) {

			imu.readIMU(sd.accelerometer, sd.gyroscope, sd.magnetometer);


#ifdef CALIBRATE
			if (calibrationCounter == samples) {
				PRINTF("Calibration of gyro: %f, %f, %f\n", gyroXSum, gyroYSum, gyroZSum);
				PRINTF("Calibration of acc: %f, %f\n", accXSum, accYSum);
				calibrationCounter++;
			} else {
				gyroXSum += sd.gyroscope.x / samples;
				gyroYSum += sd.gyroscope.y / samples;
				gyroZSum += sd.gyroscope.z / samples;

				accXSum += sd.accelerometer.x / samples;
				accYSum += sd.accelerometer.y / samples;

				calibrationCounter++;
			}
#endif

			SensorDataTopic.publish(sd, false);

			suspendUntilNextBeat();
		}
	}
};
Sense Sense("Sense");


#endif /* SENSE_SENSE_H_ */
