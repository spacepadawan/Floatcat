/*
 * power.h
 *
 *  Created on: 28.11.2017
 *      Author: Hermann
 */

#ifndef SENSE_POWER_H_
#define SENSE_POWER_H_


#include "rodos.h"
#include "../topics.h"
											//shunt resistance in ohm
#define BATTERY_ADDRESS		0b10000000		//0.002
#define MOTOR_A_ADDRESS		0b10000010		//0.05
#define MOTOR_B_ADDRESS		0b10000100		//0.05
#define MOTOR_C_ADDRESS		0b10000110		//0.05
#define MOTOR_D_ADDRESS		0b10001000		//0.05
#define SERVO1_ADDRESS		0b10001010		//0.05
#define SERVO2_ADDRESS		0b10001100		//0.05
#define SERVO3_ADDRESS		0b10001110		//0.05
#define SERVO4_ADDRESS		0b10010000		//0.05

#define READ				0b00000001

#define CONFIG_REG			0x00
#define SHUNT_VOLTAGE_REG	0x01
#define BUS_VOLTAGE_REG		0x02
#define POWER_REG			0x03
#define	CURRENT_REG			0x04
#define	CALIBRATION_REG		0x05


/*
 * Calibration of the INA219:
 * MaxPossible_I = V_shunt_max / R_shunt with V_shunt_max ~12V
 * for battery: MaxPossible_I =
 *
 */

class PowerManager {
	HAL_I2C* i2c; //HAL_I2C i2c(I2C_IDX1);
	Semaphore* sem;

	uint8_t rxBuf[2];
	uint8_t txBuf[1];

	float measureShuntVoltage(uint8_t adress);
	float measureBatteryVoltage();

public:

	PowerManager(HAL_I2C* i2c, Semaphore* sem = 0) : i2c(i2c), sem(sem) {	}

	void init();

	float readBatteryCurrent();
	float readMotorACurrent();
	float readMotorBCurrent();
	float readMotorCCurrent();
	float readMotorDCurrent();
	float readServo1Current();
	float readServo2Current();
	float readServo3Current();
	float readServo4Current();
};

/*
 * use ADC_IDX1
 */
class VoltageADC {
	HAL_ADC *adc;

public:

	VoltageADC(HAL_ADC *adc) : adc(adc) {

	}

	void init();
	float readVoltage();
};

#endif /* SENSE_POWER_H_ */
