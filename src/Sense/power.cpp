/*
 * power.cpp
 *
 *  Created on: 28.11.2017
 *      Author: Hermann
 */

#include "power.h"

void PowerManager::init() {
	if(i2c->init()) {
		PRINTF("i2c1 failed to initialize\n");
	} else {
		PRINTF("i2c1 initialized successfully\n");
	}


	sem->enter();

	txBuf[0] = CONFIG_REG;
	i2c->write(BATTERY_ADDRESS, txBuf, 1);
	txBuf[0] = 0b00011001;	//default 0b00111001
	txBuf[1] = 0b10011111;
	i2c->write(BATTERY_ADDRESS, txBuf, 2);

	txBuf[0] = CALIBRATION_REG;
	i2c->write(BATTERY_ADDRESS, txBuf, 1);
	txBuf[0] = 0x7B;
	txBuf[1] = 0x13;	// LSB 650uA
	i2c->write(BATTERY_ADDRESS, txBuf, 2);

	for (uint8_t address = MOTOR_A_ADDRESS; address <= SERVO4_ADDRESS; address++) {
		txBuf[0] = CALIBRATION_REG;
		i2c->write(address, txBuf, 1);
		txBuf[0] = 0x80;
		txBuf[1] = 0x00; // LSB 25uA
		i2c->write(address, txBuf, 2);

		txBuf[0] = CONFIG_REG;
		i2c->write(address, txBuf, 1);
		txBuf[0] = 0b00011001;	//default 0b00111001
		txBuf[1] = 0b10011111;
		i2c->write(address, txBuf, 2);
	}

	sem->leave();

}


float PowerManager::measureShuntVoltage(uint8_t address) {
	sem->enter();
	txBuf[0] = SHUNT_VOLTAGE_REG;

	i2c->write(address, txBuf, 1);
	uint16_t n = i2c->read(address | READ, rxBuf, 2);

	//uint16_t n = i2c->writeRead(address | READ, txBuf, 1, rxBuf, 2);
	sem->leave();

	//PRINTF("received: %d\n", n);

	int16_t val = static_cast<int16_t>(rxBuf[0] << 8 | rxBuf[1]);
	return (float) val * SHUNT_VOLTAGE_LSB;
}

float PowerManager::measureBusVoltage(uint8_t address) {
	sem->enter();
	txBuf[0] = BUS_VOLTAGE_REG;
	i2c->write(address, txBuf, 1);
	i2c->read(address | READ, rxBuf, 2);
	sem->leave();

	int16_t val = static_cast<int16_t>(rxBuf[0] << 5 | rxBuf[1] >> 3);
	return (float) val * BUS_VOLTAGE_LSB;
}

float PowerManager::measureCurrent(uint8_t address) {
	sem->enter();
	txBuf[0] = CURRENT_REG;

	i2c->write(address, txBuf, 1);
	uint16_t n = i2c->read(address | READ, rxBuf, 2);

	//i2c->writeRead(address | READ, txBuf, 1, rxBuf, 2);
	sem->leave();

	//PRINTF("received: %d\n", n);

	int16_t val = static_cast<int16_t>(rxBuf[0] << 8 | rxBuf[1]);

	//PRINTF("current: %d\n", val);

	return (float) val * 0.00065;
}

float PowerManager::readBatteryVoltage() {
	return measureBusVoltage(BATTERY_ADDRESS);
}

float PowerManager::readBatteryCurrent() {
	return measureShuntVoltage(BATTERY_ADDRESS) / BATTERY_SHUNT;
}

float PowerManager::readMotorACurrent() {
	return measureShuntVoltage(MOTOR_A_ADDRESS) / DEFAULT_SHUNT;
}

float PowerManager::readMotorBCurrent() {
	return measureCurrent(MOTOR_B_ADDRESS);
}

float PowerManager::readMotorCCurrent() {
	return measureCurrent(MOTOR_C_ADDRESS);
}

float PowerManager::readMotorDCurrent() {
	return measureCurrent(MOTOR_D_ADDRESS);
}

float PowerManager::readServo1Current() {
	return measureCurrent(SERVO1_ADDRESS);
}

float PowerManager::readServo2Current() {
	return measureCurrent(SERVO2_ADDRESS);
}

float PowerManager::readServo3Current() {
	return measureCurrent(SERVO3_ADDRESS);
}

float PowerManager::readServo4Current() {
	return measureCurrent(SERVO4_ADDRESS);
}

void VoltageADC::init() {
	adc->init(ADC_CH_001); ///< connect PA1 for voltage measurement
}

float VoltageADC::readVoltage() {
	uint16_t val = adc->read(ADC_CH_001);
	return (float) val;
}
