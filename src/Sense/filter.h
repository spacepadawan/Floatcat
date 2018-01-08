/*
 * filter.h
 *
 *  Created on: 19.12.2017
 *      Author: Hermann
 */

#include "matlib.h"
#include "rodos.h"
#include "../topics.h"

#ifndef SENSE_FILTER_H_
#define SENSE_FILTER_H_


class Filter {

private:

	float heading = 0;
	float factor = 0.05; // between 0 and 1, 0 means only gyro, 1 means only magnetometer

public:
	Filter() {

	}

	float filterHeading(SensorData* sd, uint64_t dt);
};


#endif /* SENSE_FILTER_H_ */
