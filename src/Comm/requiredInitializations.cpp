/*
 * requiredInitializations.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: root
 */


#include "rodos.h"
#include "hal.h"
#include "stdio.h"
#include "math.h"

#include "wifi_topics.h"


class GatewayInitiator : public Initiator {


	void init(){

	   gw.resetTopicsToForward();
	   gw.addTopicsToForward(&telemetry);
	   gw.addTopicsToForward(&telecommand);
	   //.... More Topics to come
	   //....
	   //....
	}
} gatewayInitiator;

class SystemInitialization :public Thread
{
public:

	void init ()
	{
       // Initializations of the UART (uart port and the baudrate)
	   // Initializations of the I2C and SPI interfaces.
	   //	.... etc
	   //  	.... etc

	}

	void run() {

    PRINTF("\n Hello Before Wifi INit\n");
    /*** Gateway Wifi ********/

	  wf.init("FloatSat","floatsat");
	  //wf.enableUDPConnection(0x6400A8C0,2000);  //100
	  wf.enableUDPConnection(0x6700A8C0,2000);

	          // 0x6400A8C0 = 192.168.0.100
	  	  	  // 0x6500A8C0 = 192.168.0.101
	  	  	  // 0x6600A8C0 = 192.168.0.102
	  	  	  // 0x6700A8C0 = 192.168.0.103
	  	  	  // 0x6800A8C0 = 192.168.0.104

		  // Target IP Address: 192.168.0.102 = 0xC0A80066 (reverse and hex)
		  // Target Port: 2000
    /**************************/
    PRINTF("Hi Guys From FloatSat Program All Code Working");
	}
}systemInitialization;
