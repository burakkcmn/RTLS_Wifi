/*
 * bbaDWMMessenger.h
 *
 *  Created on: May 16, 2022
 *      Author: aress
 */

#ifndef WIREDCOMMUNICATION_SERIAL_DWMCOMMUNICATIONS_BBADWMMESSENGER_H_
#define WIREDCOMMUNICATION_SERIAL_DWMCOMMUNICATIONS_BBADWMMESSENGER_H_

#include "../bbaSerial.h"

class bbaDWMMessenger final {
public:
	static void registrationRequest();
	static void startMesureRequest();
	static void getLastErrorMessageRequest();
};

#endif /* WIREDCOMMUNICATION_SERIAL_DWMCOMMUNICATIONS_BBADWMMESSENGER_H_ */
