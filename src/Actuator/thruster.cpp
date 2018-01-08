/*
 * thruster.cpp
 *
 *  Created on: 29.11.2017
 *      Author: Hermann
 */

#include "thruster.h"

void Thruster::init() {
	pin->init(true, 1, 0);
}

void Thruster::run() {
	uint64_t time;
	ThrusterPower tpBuf;

	TIME_LOOP(NOW() + id * THRUSTER_PERIOD / 3, THRUSTER_PERIOD)
	{
		time = NOW();

		thrusterPower.get(tpBuf);

		tpBuf.arr[id] = tpBuf.arr[id] > 1 ? 1 : tpBuf.arr[id];
		tpBuf.arr[id] = tpBuf.arr[id] < 0 ? 0 : tpBuf.arr[id];

		if (tpBuf.arr[id] > 0.08) {
			pin->setPins(1);
			suspendCallerUntil(time + THRUSTER_PERIOD * tpBuf.arr[id]);
		}

		if (tpBuf.arr[id] < 0.9) {
			pin->setPins(0);
		}
	}

}
