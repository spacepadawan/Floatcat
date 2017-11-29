/*
 * tools.cpp
 *
 *  Created on: 29.11.2017
 *      Author: Hermann
 */

#include "tools.h"

void delay(int64_t nanos){
	int64_t start = NOW();
	while(NOW() < start + nanos);
}
