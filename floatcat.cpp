/*
 * Template.cpp
 *
 * Created on: 25.06.2014
 * Author: Atheel Redah
 *
 */

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"

#include "src/Sense/sense.h"
#include "src/topics.h"

#include "src/Actuator/servo.h"
#include "src/Actuator/reaction_wheel.h"

static Application module01("Template", 2001);

#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063

#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define IMU_I2C I2C_IDX2

HAL_GPIO GreenLED(LED_GREEN);

float wheelDebug = 0;

class Telemetry: public Thread {

public:

	Telemetry(const char* name) : Thread(name) {
	}

	void init() {
		GreenLED.init(true, 1, 0);
		setPeriodicBeat(1 * MILLISECONDS, 500 * MILLISECONDS);
	}

	void run() {

		SensorData sd;

		while (1) {
			GreenLED.setPins(~GreenLED.readPins());

			SensorDataBuffer.get(sd);
			PRINTF("%f:\n", SECONDS_NOW());
			PRINTF("Ax = %f, Ay = %f, Az = %f\n", sd.accelerometer.x, sd.accelerometer.y, sd.accelerometer.z);
			PRINTF("Gx = %f, Gy = %f, Gz = %f\n", sd.gyroscope.x, sd.gyroscope.y, sd.gyroscope.z);
			PRINTF("Mx = %f, My = %f, Mz = %f\n", sd.magnetometer.x, sd.magnetometer.y, sd.magnetometer.z);
			//PRINTF("SERVO: %f\n", servoDebug);
			PRINTF("WHEEL: %f\n", wheelDebug);

            suspendUntilNextBeat();
		}
	}
};
Telemetry Telemetry("Telemetry");

class ReactionWheelTest: public Thread {
	ReactionWheel rw;
	float vel = 0;
	bool dir = true;
public:

	ReactionWheelTest(const char* name) : Thread(name) {
	}

	void init() {
		rw.init();
		setPeriodicBeat(2 * MILLISECONDS, 50 * MILLISECONDS);
	}

	void run() {



		while (1) {
			if (dir) {
				vel += 0.005;
				if (vel > 1) {
					vel = 1;
					dir = false;
				}
			} else {
				vel -= 0.005;
				if (vel < -1) {
					vel = -1;
					dir = true;
				}
			}

			wheelDebug = vel;

			rw.setVelocity(vel);

            suspendUntilNextBeat();
		}
	}
};
ReactionWheelTest ReactionWheelTest("ReactionWheelTest");

class ServoTest: public Thread {
private:
	Servo servo0, servo1, servo2, servo3;
	float pos[4];
	bool dir[4];

	int inc = 0;
public:

	ServoTest(const char* name) : servo0(PWM_IDX00), servo1(PWM_IDX01), servo2(PWM_IDX02), servo3(PWM_IDX03), Thread(name){
		pos[0] = 0;
		pos[1] = 0;
		pos[2] = 0;
		pos[3] = 0;
		dir[0] = false;
		dir[1] = false;
		dir[2] = false;
		dir[3] = false;
	}

	void init() {
		servo0.init(-30, 30);
		servo1.init(-30, 30);
		servo2.init(-30, 30);
		servo3.init(-30, 30);
		setPeriodicBeat(2 * MILLISECONDS, 20 * MILLISECONDS);
	}

	void run() {
		while (1) {
			for (int k = 0; k < 3; k++) {
				if (dir[k]) {
					pos[k] += 1;
					if (pos[k] > 40) {
						dir[k] = false;
						pos[k] = 40;
					}
				} else {
					pos[k] -= 1;
					if (pos[k] < -40) {
						dir[k] = true;
						pos[k] = -40;
					}
				}
			}
			servo0.write(pos[1]);
			servo1.write(pos[1]);
			servo2.write(pos[1]);
			servo3.write(pos[1]);
			suspendUntilNextBeat();
		}
	}
};
//ServoTest ServoTest("ServoTest");

/***********************************************************************/
