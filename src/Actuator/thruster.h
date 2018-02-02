/*
 * thruster.h
 *
 *  Created on: 23.11.2017
 *      Author: Hermann
 */

#ifndef ACTUATOR_THRUSTER_H_
#define ACTUATOR_THRUSTER_H_

#include "rodos.h"
#include "../topics.h"

//#define PULSE_LENGTH	(10 * MILLISECONDS)

#define THRUSTER_PERIOD	(100 * MILLISECONDS)

/*
 * 1 to H-Bridge B pinA and 2 to H-Bridge D
 */

class Thruster : Thread {
private:
	HAL_GPIO* pin;
	uint8_t id = 0;

	float power = 0;

public:
	Thruster(const char* name, HAL_GPIO* pin, uint8_t id) : pin(pin), Thread(name), id(id) { }

	/**
	 * this initializes only the pin used to toggle the specified h-bridge output. The h-bridge pwm needs
	 * to be initialized elsewhere. This method is called by rodos
	 */
	void init();
	/**
	 * as this is instantiated as an own thread, it needs to run
	 */
	void run();
};



#endif /* ACTUATOR_THRUSTER_H_ */
