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

#include "../topics.h"

#define PACK_SIZE	50


enum ControlMode {
	MANUAL = 0,
	ANGULAR_CONTROL = 1,
	POSITION_CONTROL = 2,
	MISSION_CONTROL = 3,
	UNDEFINED = 255
};

struct Telemetry
{
	float ax, ay, az, gx, gy, gz, mx, my, mz;
	float enc_rps;
	float x, y, phi;
	float v_bat, i_rw;
	float r_left, r_right;
	float star_x, star_y, star_phi;
	float x_dot, y_dot, phi_dot;
};

struct ThrusterCommand
{
	float thruster1, thruster2, thruster3;
	float servo1, servo2, servo3;
};


struct Telecommand
{
	 float data;
	 char id;
};

struct ControlCommand
{
	float x, y, phi;
	uint8_t mode;
};

struct Parameters
{
	float rw_kp, rw_ki, rw_kd;
	float ang_kp, ang_ki, ang_kd;
	float pos_kp, pos_ki, pos_kd;
	float heading_kp, heading_ki, heading_kd;

	float off1, off2, off3, off4;

	float dcdc_on;

	float gz, ax, ay;
};

extern Topic<Parameters> parameterTopicIn;
extern Topic<Parameters> parameterTopicOut;

extern Topic<RangeData> rangeSensorTopic;

extern Topic<Telemetry> telemetry;
extern Topic<Telecommand> telecommand;

extern Topic<ThrusterCommand> thrusterCommandTopic;

extern Topic<ControlCommand> controlCommandTopic;


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











