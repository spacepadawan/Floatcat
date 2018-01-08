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

#include "src/topics.h"
#include "src/Sense/power.h"

#include "src/Actuator/servo.h"
#include "src/Actuator/reaction_wheel.h"
#include "src/Actuator/thruster.h"
#include "src/Actuator/encoder.h"
#include "src/Actuator/reaction_wheel_control.h"

/*
 * UART2 - Rx:  PD6  (PA3)              Tx: PD5     (PA2)
 *         RTS: PD4  (PA1)              CTS:PD3     (PA0)
 *
 *         STM PD6 -> Rasp P8
 *         STM PD5 -> Rasp P10
 */

static Application module01("Template", 2001);

#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define IMU_I2C I2C_IDX2


HAL_GPIO h_bridge_b_pwm(GPIO_061);
HAL_GPIO h_bridge_d_pwm(GPIO_063);

HAL_GPIO thruster1Pin(GPIO_016);	//connect thruster 1 to H-Bridge B outA
HAL_GPIO thruster2Pin(GPIO_076);	//connect thruster 2 to H-Bridge D outA
HAL_GPIO thruster3Pin(GPIO_079);	//connect thruster 3 to H-Bridge D outB

HAL_PWM rw_pwm(RW_PWM_IDX);
HAL_GPIO rw_in_a(A_PIN);		// connect red to A
HAL_GPIO rw_in_b(B_PIN);		// connect black to B

HAL_GPIO DCDCOn(GPIO_066);	//PE02
HAL_GPIO PowerOK(GPIO_067); //PE03


Thruster thruster1("Thruster1", &thruster1Pin, 0),
		thruster2("Thruster2", &thruster2Pin, 1),
		thruster3("Thruster3", &thruster3Pin, 2);

ReactionWheel rw(&rw_pwm, &rw_in_a, &rw_in_b);
Servo servo1(PWM_IDX00), servo2(PWM_IDX01), servo3(PWM_IDX02), servo4(PWM_IDX03);

// TODO switch servo 1 and 2 or thruster 1 and 2

HAL_I2C i2c1(I2C_IDX1);
Semaphore sem_i2c1;

// connect white to PC7, yellow to PC6
Encoder enc;

#define ENCODER_BEAT	(10*MILLISECONDS)

class PowerThread : public Thread {
	PowerManager powerManager;

public:
	PowerThread(const char* name) : powerManager(&i2c1, &sem_i2c1) {

	}

	void init() {
	}

	void run() {
		powerManager.init();
		PowerValues p_values;

		TIME_LOOP(1 * MILLISECONDS, 500 * MILLISECONDS)
		{
			p_values.i_batt = powerManager.readBatteryCurrent();
			p_values.i_rw = powerManager.readMotorACurrent();
			p_values.v_batt = powerManager.readBatteryVoltage();

			powerValues.put(p_values);
		}
	}
};
PowerThread powerThread("PowerThread");

class ActuatorHandler : public Thread {
	float vel = 0;
	ReactionWheelController rw_cont;
public:
	ActuatorHandler(const char* name) : Thread(name), rw_cont(&rw, &enc) {}

	void init() {
		rw.init();
	}

	void run() {


		int a = enc.init(ENC_POL_NORMAL, ENC_MODE_4AB);
		//PRINTF("a: %d\n", a);



		double rps = 0;
		int64_t pos = 0;
		TIME_LOOP(NOW() + 5*MILLISECONDS, ENCODER_BEAT)
		{
			enc.read_pos();
			rps = enc.get_rps_lowpass(ENCODER_BEAT);
			rw_rps.put(rps);

			rw_cmd_power.get(vel);
			rw_cont.setDesiredRPS(vel);
			rw_cont.controlLoop(ENCODER_BEAT);

			//rw.setPower(vel);

			//PRINTF("pos: %f, rps: %f\n", pos, rps);


		}
	}
};
ActuatorHandler actuatorHandler("ActuatorHandler");


class Actuators : public Thread {

public:
	Actuators(const char* name) :
			Thread(name) {

	}

	void init() {
		DCDCOn.init(true, 1, 0);
		PowerOK.init(false, 1, 0);

		h_bridge_b_pwm.init(true, 1, 1);
		h_bridge_d_pwm.init(true, 1, 1);
	}

	void run() {
		ThrusterPower tpBuf = {0, 0, 0};
		thrusterPower.put(tpBuf);

		DCDCOn.setPins(1);
		dcdcOn = true;
	}
};
Actuators ActuatorThread("ActuatorThread");


class ServoTest: public Thread {
private:

public:

	ServoTest(const char* name) : Thread(name){

	}

	void init() {
		servo4.init(-45, 45);	//Camera Servo : -40 means looking down, +50 means looking forward
		servo1.init(-30, 30);
		servo2.init(-30, 30);
		servo3.init(-30, 30);
		setPeriodicBeat(2 * MILLISECONDS, 20 * MILLISECONDS);
	}

	void run() {
		ServoData positions = {0, 0, 0, 0};

		servo3.setBoundaries(-11, 11);

		while (1) {
			servoPositions.get(positions);
			servo1.write(positions.pos1);
			servo2.write(positions.pos2);
			servo3.write(positions.pos3);
			servo4.write(positions.pos4);
			suspendUntilNextBeat();
		}
	}
};
ServoTest ServoTest("ServoTest");

/***********************************************************************/
