/*
 * bbaSPI.cpp
 *
 *  Created on: Feb 14, 2022
 *      Author: aress
 */

#include "bbaSPI.h"
#include "../serial/bbaSerial.h"

bbaSPI::bbaSPI() {
	SPI.begin();                     // start communicating with the memory chip
	SPI.setFrequency(90000000);
}

bbaSPI::~bbaSPI() {
}

void bbaSPI::write() {
	int tempInt1 = 1453;                         // highest integer # is 32767
	EEPROM.WriteInt(100, tempInt1); // send tempInt1 to EEPROM starting from address 0
}

String bbaSPI::read() {
	String receivedMessage = "";
	int tempInt2 = EEPROM.ReadInt(100);    // Read integer from memory address 0
	if (tempInt2 != 1453) {
		bbaSerial::dbg_printf("read : %d error count : %d\n", tempInt2,
				++count);
	}
	return receivedMessage;
}
