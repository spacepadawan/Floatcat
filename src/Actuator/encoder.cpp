/*
 * encoder.cpp
 *
 *  Created on: 01.12.2017
 *      Author: Hermann
 */

#include "encoder.h"
#include "matlib.h"

#define MY_ABS(x)	((x) < 0 ? -(x) : (x))

int Encoder::init(ENC_COUNTER_POLARITY polarity, ENC_COUNTER_MODE mode) {
	ENC_TypeDef encoder;

	switch (polarity) {
	case ENC_POL_NORMAL:
		encoder.POLARITY = TIM_ICPolarity_Rising;
		break;
	case ENC_POL_REVERSE:
		encoder.POLARITY = TIM_ICPolarity_Falling;
		break;
	default:
		return -1;
	}

	switch (mode) {
	case ENC_MODE_2A:
		encoder.MODE = TIM_EncoderMode_TI1;
		coding_factor = 2;
		break;
	case ENC_MODE_2B:
		encoder.MODE = TIM_EncoderMode_TI2;
		coding_factor = 2;
		break;
	case ENC_MODE_4AB:
		coding_factor = 4;
		encoder.MODE = TIM_EncoderMode_TI12;
		break;
	default:
		return -1;
	}

	encoder.GPIO_AF = GPIO_AF_TIM3;
	encoder.RCC_Periph_TIMER = RCC_APB1Periph_TIM3;
	encoder.PIN_CH1 = ENC_TIM3_CH1_PIN;
	encoder.PIN_SOURCE_CH1 = ENC_TIM3_CH1_SOURCE;
	encoder.PORT_CH1 = ENC_TIM3_CH1_GPIO_PORT;
	encoder.RCC_Periph_GPIO_CH1 = ENC_TIM3_CH1_GPIO_CLK;
	encoder.PIN_CH2 = ENC_TIM3_CH2_PIN;
	encoder.PIN_SOURCE_CH2 = ENC_TIM3_CH2_SOURCE;
	encoder.PORT_CH2 = ENC_TIM3_CH2_GPIO_PORT;
	encoder.RCC_Periph_GPIO_CH2 = ENC_TIM3_CH2_GPIO_CLK;

	this->enc_set_gpio(encoder);
	this->enc_set_timer(encoder);

	PRINTF("Encoder initialized\n");

	return 0;
}

void Encoder::enc_set_gpio(const ENC_TypeDef &encoder)
{
	GPIO_InitTypeDef gpio_init_structure;

	// Config Pins as AF-Input
	gpio_init_structure.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_structure.GPIO_OType = GPIO_OType_PP;
	gpio_init_structure.GPIO_PuPd = GPIO_PuPd_UP;

	// Clock Enable
	RCC_AHB1PeriphClockCmd(encoder.RCC_Periph_GPIO_CH1, ENABLE);
	RCC_AHB1PeriphClockCmd(encoder.RCC_Periph_GPIO_CH2, ENABLE);

	gpio_init_structure.GPIO_Pin = encoder.PIN_CH1;
	GPIO_Init(encoder.PORT_CH1, &gpio_init_structure);
	gpio_init_structure.GPIO_Pin = encoder.PIN_CH2;
	GPIO_Init(encoder.PORT_CH2, &gpio_init_structure);

	// Alternating Function from IO-Pin
	GPIO_PinAFConfig(encoder.PORT_CH1, encoder.PIN_SOURCE_CH1, encoder.GPIO_AF);
	GPIO_PinAFConfig(encoder.PORT_CH2, encoder.PIN_SOURCE_CH2, encoder.GPIO_AF);

	PRINTF("GPIO set\n");
}


void Encoder::enc_set_timer(const ENC_TypeDef &encoder)
{
	// Clock enable
	RCC_APB1PeriphClockCmd(encoder.RCC_Periph_TIMER, ENABLE);

	// Set
	TIM_EncoderInterfaceConfig(TIMER, encoder.MODE, encoder.POLARITY, TIM_ICPolarity_Rising);

	// Set max value of counter
	TIM_SetAutoreload(TIMER, max_counts);

	// Timer enable
	TIM_Cmd(TIMER, ENABLE);

	PRINTF("timer enabled\n");

	reset_counter();
}

void Encoder::reset_counter()
{
	TIM_SetCounter(TIMER, 0);
}

void Encoder::reset()
{
	reset_counter();
	counter_dif = 0;
	counter_old = 0;
	flow_count = 0;
}

void Encoder::set_pos(int64_t degree)
{
	counter_dif = 0;
	double points = (double) ((double) degree / 360) * (coding_factor * cpr);
	flow_count = points < 0 ? points / max_counts - 1 : points / max_counts;
	points = points < 0 ? floor(points) : ceil(points);
	points -= (flow_count * max_counts);
	TIM_SetCounter(TIMER, MY_ABS(points));
	counter_old = TIM_GetCounter(TIMER);
}


void Encoder::read_pos()
{
	uint16_t counter_val = TIM_GetCounter(TIMER);

	//PRINTF("counter val: %d\n", counter_val);

	counter_dif = counter_val - counter_old;

	if (counter_old < low_counts && counter_val > high_counts)
	{
		flow_count--;
		counter_dif -= max_counts;
	}
	else if (counter_old > high_counts && counter_val < low_counts)
	{
		counter_dif += max_counts;
		flow_count++;
	}

	lastCounts[currentCountIndex] = counter_dif;
	lastCountsSum += counter_dif;
	currentCountIndex = (currentCountIndex + 1) % ENC_SAMPLES;
	lastCountsSum -= lastCounts[currentCountIndex];

	counter_old = counter_val;
}


int64_t Encoder::get_pos_rel_to_start()
{
	return (int64_t) flow_count * max_counts + TIM_GetCounter(TIMER);
}


int64_t Encoder::get_rot_deg()
{
	return (int64_t) (get_pos_rel_to_start() * 360) / (coding_factor * cpr);
}


int32_t Encoder::get_rot_speed(long long beat)
{
	return (SECONDS / beat) * 360 * counter_dif / (coding_factor * cpr);
}


int32_t Encoder::get_rot_speed_abs(long long beat)
{
	int32_t speed_abs = get_rot_speed(beat);
	return MY_ABS(speed_abs);
}


double Encoder::get_rps(long long beat)
{
	return (double) (SECONDS / beat) * counter_dif / (coding_factor * cpr);
}

double Encoder::get_rps_lowpass(long long beat) {
	return (double) (SECONDS / beat) * lastCountsSum / (coding_factor * cpr * ENC_SAMPLES);
}


double Encoder::get_rps_abs(long long beat)
{
	double rps_abs = get_rps(beat);
	return MY_ABS(rps_abs);
}
