/*
 * IMU.h
 *
 *  Created on: 04.11.2017
 *      Author: Hermann
 */

#ifndef IMU_H_
#define IMU_H_

#include "rodos.h"
#include "LSM9DS0_registers.h"
#include "matlib.h"

/*
 * Calibration:
 * 	Gyro: -7.731, 4.616, -15.964
 * 	Acc: 0.125, -0.002
 */


class IMU {
private:
	HAL_SPI* spi;
	HAL_GPIO *csXM;
	HAL_GPIO *csG;
	HAL_GPIO *imuEN;

	Semaphore* sem;

	uint8_t rxBuf[6] = { 0 };
	uint8_t txBuf[3] = { 0 };

	int16_t err = 0;

	int32_t writeSPI(HAL_GPIO *cs, const uint8_t* txBuf, uint32_t txBufSize);
    int32_t writeReadSPI(HAL_GPIO *cs, const uint8_t* txBuf, uint32_t txBufSize, uint8_t* rxBuf, uint32_t rxBufSize);

    bool readAccel(RODOS::Vector3D_F &data);
    bool readGyro(RODOS::Vector3D_F &data);
    bool readMag(RODOS::Vector3D_F &data);

public:
    IMU(HAL_SPI* spi, HAL_GPIO *csXM, HAL_GPIO *csG, HAL_GPIO *imuEN = NULL, Semaphore* sem = 0) {
    	this->spi = spi;
    	this->csXM = csXM;
    	this->csG = csG;
    	this->imuEN = imuEN;
    	this->sem = sem;
    }

    bool init();
    void reset();
    bool readIMU(RODOS::Vector3D_F &accelerometer, RODOS::Vector3D_F &gyroscope, RODOS::Vector3D_F &magnetometer);
};





#endif /* IMU_H_ */
