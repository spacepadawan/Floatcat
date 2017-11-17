/*
 * tc.h
 *
 *  Created on: 09.11.2017
 *      Author: Hermann
 */

#ifndef TC_TC_H_
#define TC_TC_H_

#include "rodos.h"
#include "..\topics.h"

class Telecommand: public Thread {

public:

	Telecommand(const char* name) : Thread(name) {
	}

	void init() {
		setPeriodicBeat(0, 500 * MILLISECONDS);
	}

	void run() {



		while (1) {



            suspendUntilNextBeat();
		}
	}
};
Telecommand Telecommand("Telecommand");



#endif /* TC_TC_H_ */
