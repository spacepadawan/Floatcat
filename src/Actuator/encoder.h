/*
 * encoder.h
 *
 *  Created on: 01.12.2017
 *      Author: Hermann
 */

#ifndef ACTUATOR_ENCODER_H_
#define ACTUATOR_ENCODER_H_

#include "rodos.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

/*
 * TIM3 is used -> connect yellow to PC6 and white to PC7
 */

#define ENC_TIM3_CH1_GPIO_PORT         GPIOC
#define ENC_TIM3_CH1_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ENC_TIM3_CH1_PIN               GPIO_Pin_6
#define ENC_TIM3_CH1_SOURCE            GPIO_PinSource6

#define ENC_TIM3_CH2_GPIO_PORT         GPIOC
#define ENC_TIM3_CH2_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ENC_TIM3_CH2_PIN               GPIO_Pin_7
#define ENC_TIM3_CH2_SOURCE            GPIO_PinSource7

/*
 * TIM2 is used -> connect to PA15 and PA3
 */

#define ENC_TIM2_CH1_GPIO_PORT         GPIOA
#define ENC_TIM2_CH1_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENC_TIM2_CH1_PIN               GPIO_Pin_15
#define ENC_TIM2_CH1_SOURCE            GPIO_PinSource15

#define ENC_TIM2_CH2_GPIO_PORT         GPIOB
#define ENC_TIM2_CH2_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define ENC_TIM2_CH2_PIN               GPIO_Pin_3
#define ENC_TIM2_CH2_SOURCE            GPIO_PinSource3

//--------------------------------------------------------------
// Mode of Encoder
//--------------------------------------------------------------
typedef enum {
	ENC_MODE_2A,	// Counter counts at edges of input CH1 (A)
	ENC_MODE_2B,	// Counter counts at edges of input CH2 (B)
	ENC_MODE_4AB	// Counter counts at edges of input CH1 (A) AND input CH2 (B)
}ENC_COUNTER_MODE;


//--------------------------------------------------------------
// Direction of Encoder
//--------------------------------------------------------------
typedef enum {
	ENC_POL_NORMAL,	// Counter increments at counter-clockwise turning
					// for Rotary Encoder Model J733
					// when wired white cable to CH1 GPIO and green cable to CH2 GPIO
	ENC_POL_REVERSE	// Counter decrements at counter-clockwise turning
					// for Rotary Encoder Model J733
					// when wired white cable to CH1 GPIO and green cable to CH2 GPIO
}ENC_COUNTER_POLARITY;

//--------------------------------------------------------------
// Structure of Encoder-Parameters
//--------------------------------------------------------------
typedef struct {
	  uint8_t GPIO_AF;
	  GPIO_TypeDef* PORT_CH1;
	  GPIO_TypeDef* PORT_CH2;
	  uint16_t PIN_CH1;
	  uint16_t PIN_CH2;
	  uint8_t PIN_SOURCE_CH1;
	  uint8_t PIN_SOURCE_CH2;
	  uint32_t RCC_Periph_GPIO_CH1;
	  uint32_t RCC_Periph_GPIO_CH2;
	  uint32_t RCC_Periph_TIMER;
	  uint16_t MODE;	// Mode determines at which edges counter is counting
	  uint16_t POLARITY;
}ENC_TypeDef;

#define ENC_SAMPLES		16

class Encoder {
private:
	TIM_TypeDef* TIMER = TIM3;
	uint16_t cpr = 16, //Counts per rotation
	max_counts = 65535 / cpr * cpr,
	high_counts = 60000,
	low_counts = 5535,
	counter_old = 0,
	flow_count = 0,
	coding_factor = 0;
	int16_t counter_dif = 0;

	int16_t lastCounts[ENC_SAMPLES] = {0};
	int32_t lastCountsSum = 0;
	uint8_t currentCountIndex = 0;

	void enc_set_gpio(const ENC_TypeDef &encoder);
	void enc_set_timer(const ENC_TypeDef &encoder);
	void reset_counter();

public:
	Encoder() { }

	int init(ENC_COUNTER_POLARITY polarity, ENC_COUNTER_MODE mode);
	void reset();

	void set_pos(int64_t degree);

	/*
	 * This function shall be called at the beginning of loop iteration
	 * and is necessary for the getter functions of this class to return proper values
	 * Identifies underflow/overflow of counter and computes the difference of counter value
	 * from previous iteration call and current iteration call
	 */
	void read_pos();

	/*
	 * In order to get proper value call ONLY ONCE (for each iteration) read_pos() at the beginning of loop iteration
	 * Returns Counter value relative to start position
	 */
	int64_t get_pos_rel_to_start();

	/*
	 * In order to get proper value call ONLY ONCE (for each iteration) read_pos() at the beginning of loop iteration
	 * @param long long beat: Specify period duration of loop iteration (Sample Time)
	 * Returns Rotation speed in revolutions per second
	 */
	double get_rps(long long beat);

	/*
	 * In order to get proper value call ONLY ONCE (for each iteration) read_pos() at the beginning of loop iteration
	 * @param long long beat: Specify period duration of loop iteration (Sample Time)
	 * Returns Rotation speed in revolutions per second
	 */
	double get_rps_abs(long long beat);

	/*
	 * In order to get proper value call ONLY ONCE (for each iteration) read_pos() at the beginning of loop iteration
	 * @param long long beat: Specify period duration of loop iteration (Sample Time)
	 * Returns Rotation speed in revolutions per second after feeding it through a lowpass filter
	 */
	double get_rps_lowpass(long long beat);
};

#endif /* ACTUATOR_ENCODER_H_ */
