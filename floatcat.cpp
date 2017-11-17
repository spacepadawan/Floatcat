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

static Application module01("Template", 2001);

#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063

#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define IMU_I2C I2C_IDX2

HAL_GPIO GreenLED(LED_GREEN);


class Telemetry: public Thread {

public:

	Telemetry(const char* name) : Thread(name) {
	}

	void init() {
		GreenLED.init(true, 1, 0);
		setPeriodicBeat(0, 500 * MILLISECONDS);
	}

	void run() {

		SensorData sd;

		while (1) {
			GreenLED.setPins(~GreenLED.readPins());

			SensorDataBuffer.get(sd);
			PRINTF("%f:\n", SECONDS_NOW());
			PRINTF("Ax = %f, Ay = %f, Az = %f\n", sd.accelerometer.x, sd.accelerometer.y, sd.accelerometer.z);
			PRINTF("Gx = %f, Gy = %f, Gz = %f\n", sd.gyroscope.x, sd.gyroscope.y, sd.gyroscope.z);
			PRINTF("Mx = %f, My = %f, Mz = %f\r\n", sd.magnetometer.x, sd.magnetometer.y, sd.magnetometer.z);


            suspendUntilNextBeat();
		}
	}
};
Telemetry Telemetry("Telemetry");

/***********************************************************************/
