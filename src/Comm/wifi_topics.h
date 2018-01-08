/*****************************************************************
topics.h

Original Created by: Atheel Redah @ University of W�rzburg
Original Creation Date: March 8, 2015

Development environment specifics:
	Software Platform: Rodos (Realtime Onboard Dependable Operating System).
	Hardware Platform: STM32F4 + W�rzburg Uni Informatik 8 Discovery AddOn Board Version 2.0.
*****************************************************************/

#ifndef __topics_h__
#define __topics_h_



/* Includes ------------------------------------------------------------------*/
#include "rodos.h"

#include "wifi/drivers/esp8266/ESP8266.h"
#include "wifi/drivers/wf121/wf121.h"
#include "wifi/linkinterfacewifi.h"

#define PACK_SIZE	50


struct Telemetry
{
	float ax, ay, az, gx, gy, gz, mx, my, mz;
	float enc_rps;
	float x, y, phi;
	float v_bat, i_rw;
};


struct Telecommand
{
	 float data;
	 char id;
};

extern Topic<Telemetry> telemetry;
extern Topic<Telecommand> telecommand;


extern HAL_UART gatewayWifi; // USB-UART
extern LinkinterfaceWifi linkifwf;
extern Gateway gw;

//#define Wifi_WF121
#define Wifi_ESP8266


#ifdef Wifi_WF121
extern WF121 wf;
#endif

#ifdef Wifi_ESP8266
extern ESP8266 wf;
#endif

extern HAL_GPIO gpio0;
extern HAL_GPIO enable;


//#endif


#endif











