/*
 * bbaSPI.h
 *
 *  Created on: Feb 14, 2022
 *      Author: aress
 */

#ifndef WIREDCOMMUNICATION_BBASPI_H_
#define WIREDCOMMUNICATION_BBASPI_H_

#include <Arduino.h>
#include "spiEEPROM.h"

#define CSPIN 15       // Default Chip Select Line for Uno (change as needed)

class bbaSPI {
public:
	bbaSPI();
	virtual ~bbaSPI();
	void write();
	String read();

private:
	spiEEPROM EEPROM;       //initialize an instance of this class
	int count = 0;
};

#endif /* WIREDCOMMUNICATION_BBASPI_H_ */
