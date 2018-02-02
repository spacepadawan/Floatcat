/*
 * rangesensors.h
 *
 *  Created on: 29.01.2018
 *      Author: Hermann
 */

#ifndef SENSE_RANGESENSORS_CPP_
#define SENSE_RANGESENSORS_CPP_


#include "rodos.h"
#include "VL6180X.h"
#include "../topics.h"

HAL_GPIO enableLeft(GPIO_033); //PC01
HAL_GPIO enableRight(GPIO_035); //PC03

HAL_I2C	i2c2(I2C_IDX2);
Semaphore sem_i2c2;

uint8_t leftAdress = 0x29;
uint8_t rightAdress = 0x30;

// Create both sensors with same address because this is the default one
VL6180x sensorLeft(leftAdress, &i2c2, &sem_i2c2), sensorRight(leftAdress, &i2c2, &sem_i2c2);

class RangeSensorHandler : public Thread{



public:
	RangeSensorHandler(const char* name) : Thread(name) {

	}

	void init() {
		enableLeft.init(true, 1, 0);
		enableRight.init(true, 1, 0);

		i2c2.init(400000);
	}

	void run() {
		enableRight.setPins(1);

		suspendCallerUntil(NOW() + 10 * MILLISECONDS);

		if (sensorRight.VL6180xInit()) {
			//PRINTF("failed right sensor init\n");
		}

		if (sensorRight.changeAddress(leftAdress, rightAdress) == rightAdress) {
			//PRINTF("adress changed\n");
		}

		suspendCallerUntil(NOW() + 10 * MILLISECONDS);

		enableLeft.setPins(1);

		suspendCallerUntil(NOW() + 10 * MILLISECONDS);

		if (sensorLeft.VL6180xInit()) {
			//PRINTF("failed right sensor init\n");
		}

		TIME_LOOP(NOW(), 500 * MILLISECONDS) {
			RangeData rd;
			rd.left = sensorLeft.getDistance();
			suspendCallerUntil(NOW() + 5 * MILLISECONDS);
			rd.right = sensorRight.getDistance();
			//PRINTF("left: %d, right %d \n", rd.left, rd.right);
			rangeSensorTopic.publish(rd);
		}
	}
};

RangeSensorHandler rangeSensorHandler("IR Sensor Handler");


#endif /* SENSE_RANGESENSORS_CPP_ */
