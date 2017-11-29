/*
 * h_bridge.h
 *
 *  Created on: 23.11.2017
 *      Author: Hermann
 */

#ifndef ACTUATOR_H_BRIDGE_H_
#define ACTUATOR_H_BRIDGE_H_

enum H_BRIDGE_IDX {
	H_BRIDGE_A = 0,
	H_BRIDGE_B,
	H_BRIDGE_C,
	H_BRIDGE_D
};

/*
 * RODOS api is lying, pin assignments for the PWMs are actually like this:
 */

#define BR_MOT_TIM_A	PWM_IDX12	// PWM A	Timer 4 CH 1
#define BR_MOT_IOA_A	GPIO_036	// INA A	STM32F4: PC4
#define BR_MOT_IOB_A	GPIO_017	// INB A	STM32F4: PB1

#define BR_MOT_TIM_B	PWM_IDX13	// PWM B	Timer 4 CH 2
#define BR_MOT_IOA_B	GPIO_016	// INA B	STM32F4: PB0
#define BR_MOT_IOB_B	GPIO_071	// INB B	STM32F4: PE7

#define BR_MOT_TIM_C	PWM_IDX14	// PWM C	Timer 4 CH 3
#define BR_MOT_IOA_C	GPIO_072	// INA C	STM32F4: PE8
#define BR_MOT_IOB_C	GPIO_074	// INB C	STM32F4: PE10

#define BR_MOT_TIM_D	PWM_IDX15	// PWM D	Timer 4 CH 4
#define BR_MOT_IOA_D	GPIO_076	// INA D	STM32F4: PE12
#define BR_MOT_IOB_D	GPIO_079	// INB D	STM32F4: PE15

#endif /* ACTUATOR_H_BRIDGE_H_ */
