/*
 * pose.h
 *
 *  Created on: 29.01.2018
 *      Author: Hermann
 */

#ifndef MISC_POSE_H_
#define MISC_POSE_H_

#include "matlib.h"

struct Pose {
	float x, y, phi;

	Pose diff(Pose other) {
		Pose out;
		out.x = x - other.x;
		out.y = y - other.y;
		out.phi = phi - other.phi;
		return out;
	}

	Pose add(Pose other) {
		Pose out;
		out.x = x + other.x;
		out.y = y + other.y;
		out.phi = phi + other.phi;
		return out;
	}

	Pose scale(float factor) {
		Pose out;
		out.x = x * factor;
		out.y = y * factor;
		out.phi = phi * factor;
		return out;
	}

	Pose rotate(float angle) {
		Pose local;

		float s = sin(angle * M_PI / 180);
		float c = cos(angle * M_PI / 180);

		local.x = c * x + s * y;
		local.y = c * y - s * x;
		local.phi = phi;

		return local;
	}

	Pose operator-(Pose other) { return diff(other); }
	Pose operator+(Pose other) { return add(other); }
	Pose operator*(float factor) { return scale(factor); }
};


#endif /* MISC_POSE_H_ */
