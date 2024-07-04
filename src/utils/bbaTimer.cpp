/*
 * bbaTimer.cpp
 *
 *  Created on: Mar 23, 2023
 *      Author: aress
 */

#include "bbaTimer.h"
#include "../wiredCommunication/serial/bbaSerial.h"

bbaTimer::bbaTimer() {
}

bbaTimer::~bbaTimer() {
}

bool bbaTimer::check(const long &interval) {
	unsigned long current_time = millis();
	if (current_time - previous_time >= interval) {
		previous_time = current_time;
		return true;
	}
	return false;
}
