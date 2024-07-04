/*
 * bbaTimer.h
 *
 *  Created on: Mar 23, 2023
 *      Author: aress
 */

#ifndef UTILS_BBATIMER_H_
#define UTILS_BBATIMER_H_

#include <Arduino.h>

class bbaTimer final {
private:
	unsigned long previous_time = millis();

public:
	bbaTimer();
	virtual ~bbaTimer();

	bool check(const long &interval);
};

#endif /* UTILS_BBATIMER_H_ */
