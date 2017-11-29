/*
 * reaction_wheel.h
 *
 *  Created on: 23.11.2017
 *      Author: Hermann
 */

#ifndef ACTUATOR_REACTION_WHEEL_H_
#define ACTUATOR_REACTION_WHEEL_H_

#include "rodos.h"

#define MY_ABS(x)	((x) < 0 ? -(x) : (x))
#define EPSILON		0.001

/*
 * RODOS api is lying
 *
 * 	PD12	T4_CH1	H-Bridge A
 *	PD13	T4_CH2	H-Bridge B
 *	PD14	T4_CH3	H-Bridge C
 *	PD15	T4_CH4	H-Bridge D
 */

#define RW_PWM_IDX 	PWM_IDX12 // Reaction wheel has to be connected to H-Bridge A

/*
 *  PC4		GPIO_036	H-Bridge A inA
 *  PB1		GPIO_017	H-Bridge A inB
 */

#define A_PIN	GPIO_036
#define B_PIN	GPIO_017

/*
 * 84MHz / 1000 steps = 84000 increments possible
 */
#define RW_FREQUENCY	1000
#define RW_INCREMENTS	8400

extern HAL_PWM pwm;
extern HAL_GPIO in_a;
extern HAL_GPIO in_b;

class ReactionWheel {
private:

public:
	ReactionWheel();

	void init();
	void setVelocity(float power);
};


#endif /* ACTUATOR_REACTION_WHEEL_H_ */
