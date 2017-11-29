/*
 * power.cpp
 *
 *  Created on: 28.11.2017
 *      Author: Hermann
 */

#include "power.h"

void PowerManager::init() {
	i2c->init();
	/*
	sem->enter();
	txBuf[0] = CONFIG_REG;
	i2c->write(BATTERY_ADDRESS, txBuf, 1);
	txBuf[0] = 0b00111001;
	txBuf[1] = 0b10011111;
	i2c->write(BATTERY_ADDRESS, txBuf, 2);
	sem->leave();
	*/
}


float PowerManager::measureShuntVoltage(uint8_t adress) {
	sem->enter();
	txBuf[0] = SHUNT_VOLTAGE_REG;
	i2c->writeRead(adress | READ, txBuf, 1, rxBuf, 2);
	sem->leave();

	uint16_t val = static_cast<int16_t>(rxBuf[0] << 8 | rxBuf[1]);
	return (float) val;
}

float PowerManager::readBatteryCurrent() {
	return measureShuntVoltage(BATTERY_ADDRESS);
}

float PowerManager::readMotorACurrent() {
	return measureShuntVoltage(MOTOR_A_ADDRESS);
}

float PowerManager::readMotorBCurrent() {
	return measureShuntVoltage(MOTOR_B_ADDRESS);
}

float PowerManager::readMotorCCurrent() {
	return measureShuntVoltage(MOTOR_C_ADDRESS);
}

float PowerManager::readMotorDCurrent() {
	return measureShuntVoltage(MOTOR_D_ADDRESS);
}

float PowerManager::readServo1Current() {
	return measureShuntVoltage(SERVO1_ADDRESS);
}

float PowerManager::readServo2Current() {
	return measureShuntVoltage(SERVO2_ADDRESS);
}

float PowerManager::readServo3Current() {
	return measureShuntVoltage(SERVO3_ADDRESS);
}

float PowerManager::readServo4Current() {
	return measureShuntVoltage(SERVO4_ADDRESS);
}

void VoltageADC::init() {
	adc->init(ADC_CH_001); ///< connect PA1 for voltage measurement
}

float VoltageADC::readVoltage() {
	uint16_t val = adc->read(ADC_CH_001);
	return (float) val;
}
