/*
 * IMU.cpp
 *
 *  Created on: 06.11.2017
 *      Author: Hermann
 */

#include "imu.h"
#include "../Misc/tools.h"

RODOS::Vector3D_F calibG(6.056, -4.156, 13.151);
RODOS::Vector3D_F calibAcc(-0.233, 0.005, 0);

IMU::IMU(HAL_SPI* spi, HAL_GPIO *csXM, HAL_GPIO *csG, HAL_GPIO *imuEN, Semaphore* sem) :
	spi(spi), csXM(csXM), csG(csG), imuEN(imuEN), sem(sem) {

}

int32_t IMU::writeSPI(HAL_GPIO *cs, const uint8_t* txBuf, uint32_t txBufSize) {
	if (sem != NULL && Scheduler::isSchedulerRunning())
		sem->enter();
	cs->setPins(0);
	int32_t res = spi->write(txBuf, txBufSize);
	cs->setPins(1);
	if (sem != NULL && Scheduler::isSchedulerRunning())
		sem->leave();
	return res;
}

int32_t IMU::writeReadSPI(HAL_GPIO *cs, const uint8_t* txBuf, uint32_t txBufSize, uint8_t* rxBuf, uint32_t rxBufSize) {
	if (sem != NULL && Scheduler::isSchedulerRunning())
		sem->enter();
	cs->setPins(0);
	int32_t res = spi->write(txBuf, txBufSize);
	res += spi->read(rxBuf, rxBufSize);
	cs->setPins(1);
	if (sem != NULL && Scheduler::isSchedulerRunning())
		sem->leave();
	return res;
}

bool IMU::init(){
	//spi->init();
	csXM->init(true, 1, 1);
	csG->init(true, 1, 1);
	if (imuEN != NULL)
		imuEN->init(true, 1, 1);

	int xm_id = 0, g_id = 0;
	txBuf[0] = (LSM9DS0_WHO_AM_I_XM | READ);
	err = writeReadSPI(csXM, txBuf, 1, rxBuf, 1);
	xm_id = rxBuf[0];

	txBuf[0] = (LSM9DS0_WHO_AM_I_G | READ);
	err += writeReadSPI(csG, txBuf, 1, rxBuf, 1);
	g_id = rxBuf[0];

	// Enable accel
	txBuf[0] = LSM9DS0_CTRL_REG1_XM;
	txBuf[1] = LSM9DS0_XM_ODR_200HZ;
	err += writeSPI(csXM, txBuf, 2);

	txBuf[0] = LSM9DS0_CTRL_REG2_XM;
	txBuf[1] = 0xC0; // 50hz anti-alias-filter
	err += writeSPI(csXM, txBuf, 2);

	txBuf[0] = LSM9DS0_CTRL_REG5_XM;
	txBuf[1] = 0xF0;
	err += writeSPI(csXM, txBuf, 2);

	// Enable mag
	txBuf[0] = LSM9DS0_CTRL_REG7_XM;
	txBuf[1] = 0x00;
	err += writeSPI(csXM, txBuf, 2);

	// Enable temp
	txBuf[0] = LSM9DS0_CTRL_REG5_XM | READ;
	err += writeReadSPI(csXM, txBuf, 1, rxBuf, 1);

	txBuf[0] = LSM9DS0_CTRL_REG5_XM;
	txBuf[1] = rxBuf[0] | 0xF4; // Enable Temp, High resoultion mag, 100Hz ODR
	err += writeSPI(csXM, txBuf, 2);

	/*
	// set DPS Gyro to 500
	txBuf[0] = LSM9DS0_CTRL_REG4_G | READ;
	err += writeReadSPI(txBuf, 1, rxBuf, 1);

	txBuf[0] = LSM9DS0_CTRL_REG4_G;
	rxBuf[0] &= ~(0b00110000);
	txBuf[1] = rxBuf[0] | 0x10;
	err += writeSPI(txBuf, 2);
	 */

	// Enable Gyro
	txBuf[0] = LSM9DS0_CTRL_REG1_G;
	txBuf[1] = LSM9DS0_G_ODR_190HZ;		// 190 Hz , 12.5 BW
	err += writeSPI(csG, txBuf, 2); // on XYZ

	if (xm_id != LSM9DS0_XM_ID || g_id != LSM9DS0_G_ID) { //xm_id-->73, g_id-->212
		PRINTF("IMU ERROR 0x%x 0x%x 0x%x\n", err, xm_id, g_id);
		return false;
	} else {
		return true;
	}
}

void IMU::reset(){
	if (imuEN != NULL){
		imuEN->setPins(0);
		delay(50 * MICROSECONDS);
	}
	spi->reset();
	if (imuEN != NULL)
		imuEN->setPins(1);
	spi->init();
}

bool IMU::readAccel(RODOS::Vector3D_F &data) {
	txBuf[0] = LSM9DS0_OUT_X_L_A | READ | INCREMENT;

	err = writeReadSPI(csXM, txBuf, 1, rxBuf, 6);

	data.x = (static_cast<int16_t>(rxBuf[1] << 8 | rxBuf[0])
			* LSM9DS0_ACCEL_MG_LSB_2G) / 1000;
	data.y = (static_cast<int16_t>(rxBuf[3] << 8 | rxBuf[2])
			* LSM9DS0_ACCEL_MG_LSB_2G) / 1000;
	data.z = (static_cast<int16_t>(rxBuf[5] << 8 | rxBuf[4])
			* LSM9DS0_ACCEL_MG_LSB_2G) / 1000;

	//data = data.vecSub(calibAcc);

	//data = rot * data;

	return (err == 7);
}

bool IMU::readGyro(RODOS::Vector3D_F &data) {
	txBuf[0] = LSM9DS0_OUT_X_L_G | READ | INCREMENT;
	err = writeReadSPI(csG, txBuf, 1, rxBuf, 6);

	data.x = static_cast<int16_t>(rxBuf[1] << 8 | rxBuf[0])
			* LSM9DS0_GYRO_DPS_DIGIT_245DPS;
	data.y = static_cast<int16_t>(rxBuf[3] << 8 | rxBuf[2])
			* LSM9DS0_GYRO_DPS_DIGIT_245DPS;
	data.z = static_cast<int16_t>(rxBuf[5] << 8 | rxBuf[4])
			* LSM9DS0_GYRO_DPS_DIGIT_245DPS;

	//data = data.vecSub(calibG);

	//data = rot * data;

	return (err == 7);
}

bool IMU::readMag(RODOS::Vector3D_F &data) {
	txBuf[0] = LSM9DS0_OUT_X_L_M | READ | INCREMENT;
	err = writeReadSPI(csXM, txBuf, 1, rxBuf, 6);

	data.x = (static_cast<int16_t>(rxBuf[1] << 8 | rxBuf[0])
			* LSM9DS0_MAG_MGAUSS_4GAUSS) / 1000;
	data.y = (static_cast<int16_t>(rxBuf[3] << 8 | rxBuf[2])
			* LSM9DS0_MAG_MGAUSS_4GAUSS) / 1000;
	data.z = -(static_cast<int16_t>(rxBuf[5] << 8 | rxBuf[4])
			* LSM9DS0_MAG_MGAUSS_4GAUSS) / 1000;

	//data = rot * data;

	return (err == 7);
}

bool IMU::readIMU(RODOS::Vector3D_F &accelerometer, RODOS::Vector3D_F &gyroscope, RODOS::Vector3D_F &magnetometer){
	return readAccel(accelerometer) && readGyro(gyroscope) && readMag(magnetometer);
}
