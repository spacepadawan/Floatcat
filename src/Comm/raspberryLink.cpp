/*
 * raspberryLink.cpp
 *
 *  Created on: 16.01.2018
 *      Author: Hermann
 */

#include "rodos.h"
#include <stdlib.h>
#include "../topics.h"

enum LinkState {
	NEW_MSG, ID, ARGUMENT
};

#define LINK_PERIOD			500 * MILLISECONDS
#define MESSAGE_TIMEOUT		5 * SECONDS

#define ID_LENGTH			2
#define ARGUMENT_LENGTH		10

#define MAX_ARGUMENTS		4

#define START_CHARACTER		'$'
#define END_CHARACTER		'\n'
#define DELIMITER			':'

enum MessageType {
	ST, OT, RD, UNDEFINED
};

struct Message {
	MessageType type;
	float args[MAX_ARGUMENTS];
	uint8_t numberOfArguments;
	int64_t timeStamp;
};

inline void handleMessage(Message &msg) {
	switch(msg.type) {
	case ST:
		if (msg.numberOfArguments < 3) {
			return;
		}

		Pose p;

		p.x = msg.args[0];
		p.y = msg.args[1];
		p.phi = msg.args[2];

		//PRINTF("Star Tracker: (%f, %f, %f)\n", p.x, p.y, p.phi);

		starTrackerPoseTopic.publish(p);
		break;
	case OT:

		break;
	case RD:

		break;
	}
}

CommBuffer<Message> raspberryMessageBuffer;

MessageType getMessageType(char c0, char c1) {
	if (c0 == 'S' && c1 == 'T') {
		return ST;
	}
	if (c0 == 'O' && c1 == 'T') {
		return OT;
	}
	if (c0 == 'R' && c1 == 'D') {
		return RD;
	}
	return UNDEFINED;
}

HAL_UART uart_stdout(UART_IDX5);	//                tx-PC12   rx-PD2

#define USART_BUFFER_SIZE				100
char usart_buffer[USART_BUFFER_SIZE];

class RaspberryLink: public Thread {
	char received;
	int64_t start;

	LinkState state;
	bool valid, corrupt, timeout, dot;

	uint8_t currentReadIndex;
	uint8_t currentArgIndex;

	char id[ID_LENGTH];
	char data[ARGUMENT_LENGTH];

	float arguments[MAX_ARGUMENTS];

public:
	RaspberryLink(const char* name) :
		Thread(name) {

	}

	void resetTC() {
		state = NEW_MSG;
		currentReadIndex = 0;
		currentArgIndex = 0;
		valid = false;
		corrupt = false;
		timeout = false;
		dot = false;

		start = END_OF_TIME - 1 * SECONDS;
	}

	void init() {
		uart_stdout.init(115200);
		resetTC();
	}

	void run() {

		int64_t time;

		this->setPeriodicBeat(1 * MILLISECONDS, LINK_PERIOD);

		while (1) {
			time = NOW();

			if (time - start < MESSAGE_TIMEOUT) {
				timeout = true;
				resetTC();
			}

			//uart_stdout.suspendUntilDataReady();
			while (uart_stdout.read(&received, 1)) {
				//PRINTF("%c", received);
				switch (state) {
				case NEW_MSG:
					if (received == START_CHARACTER) {
						start = time;
						state = ID;
						//qDebug() << "start reading";
					} else {
						corrupt = true;
						//qDebug() << "start expected";
					}
					break;
				case ID:

					if (currentReadIndex < ID_LENGTH) {
						id[currentReadIndex] = received;
						currentReadIndex++;
						//qDebug() << "character added to ID";
					} else {
						if (received == DELIMITER) {
							state = ARGUMENT;
							currentReadIndex = 0;
							//qDebug() << "ID complete";
						} else {
							corrupt = true;
							//qDebug() << "delimiter expected after ID";
						}
					}
					break;
				case ARGUMENT:
					if (received == DELIMITER || received == END_CHARACTER) {
						if (currentReadIndex == 0) {
							//qDebug() << "Argument can't be empty";
							corrupt = true;
							break;
						}

						if (currentReadIndex < ARGUMENT_LENGTH - 1) {
							//PRINTF(data);
							//PRINTF("Filling up the argument buffer\n");
							if (!dot) {
								data[currentReadIndex++] = '.';
								//PRINTF("Dot has been added\n");
							}
							while (currentReadIndex < ARGUMENT_LENGTH) {
								data[currentReadIndex++] = '0';
							}
						}
						//PRINTF(data);
						//PRINTF("\n");

						arguments[currentArgIndex] = atof(data);
						/*
						if (data[0] == '-') {
							arguments[currentArgIndex] *= -1;
						}
						*/
						//qDebug() << data[0] << data[1] << data[2] << data[3] << data[4] << data[5] << data[6] << data[7] << data[8] << data[9];

						if (received == DELIMITER) {
							currentArgIndex++;
							if (currentArgIndex >= MAX_ARGUMENTS) {
								//qDebug() << "too many arguments";
								corrupt = true;
							}
							//qDebug() << "next argument";

							dot = false;
							currentReadIndex = 0;
							break;

						} else if (received == END_CHARACTER) {

							// finish it
							Message msg;
							msg.type = getMessageType(id[0], id[1]);

							msg.args[0] = arguments[0];
							msg.args[1] = arguments[1];
							msg.args[2] = arguments[2];
							msg.args[3] = arguments[3];

							msg.numberOfArguments = currentArgIndex + 1;
							msg.timeStamp = start;

							handleMessage(msg);

							//qDebug() << "OK";
							//qDebug() << "ID: " << id[0] << id[1] << "arg0: " << arguments[0] << "arg1: " << arguments[1] << "arg2: " << arguments[2];
							//qDebug() << "Number of arguments:" << msg.numberOfArgs;
							resetTC();

							break;
						}
					}

					if (currentReadIndex < ARGUMENT_LENGTH) {
						//qDebug() << "reading Number";
						// if there is a minus inbetween a number, it must be corrupt

						data[currentReadIndex] = received;
						if (received == '.') {
							//data[currentReadIndex] = '.';
							if (dot) {
								//qDebug() << "too many dots in one number";
								corrupt = true;
								break;
							} else {
								dot = true;
								//PRINTF("Dot detected");
							}

						}

						if (!(received == '0' || received == '1' || received == '2' || received == '3' || received == '4' || received == '5' || received == '6' || received == '7' || received == '8' || received == '9')) {
							if (received == '-' && currentReadIndex > 0 ) {
								corrupt = true;
								break;
							}
						}
						currentReadIndex++;
					} else {
						corrupt = true;
						//qDebug() << "too long";
					}

					break;
				}

				if (corrupt) {
					//qDebug() << "ERROR";
					PRINTF("Corrupt\n");
					resetTC();
				}
			}

			RaspberryCommand cmd;
			if (raspberryCMDBuffer.getOnlyIfNewData(cmd)) {

				switch (cmd) {
				case CMD_START_STARTRACKER:
					uart_stdout.write("$ST1\n", 5);
					break;
				case CMD_START_OBJECTDETECTION:
					uart_stdout.write("$OT1\n", 5);
					break;
				case CMD_START_RADIO:
					uart_stdout.write("$RD1\n", 5);
					break;
				case CMD_PAUSE:
					uart_stdout.write("$OT0\n", 5);
					uart_stdout.write("$ST0\n", 5);
					uart_stdout.write("$RD0\n", 5);
					break;
				}
			}

			suspendUntilNextBeat();
		}

	}
};

RaspberryLink RaspberryLink("RaspberryLink");
