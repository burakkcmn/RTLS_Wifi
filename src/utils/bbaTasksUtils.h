/*
 * bbaTasksUtils.h
 *
 *  Created on: 5 Eyl 2023
 *      Author: burak
 */

#ifndef UTILS_BBATASKSUTILS_H_
#define UTILS_BBATASKSUTILS_H_

#include "../wiredCommunication/gpio/bbaEspGpioController.h"
#include <Ticker.h>

class bbaTasksUtils {
private:
	Ticker timer;
	uint8_t count;
	bbaEspGpioController espGpioController;
	bool wifiLedState;

	void wifiLedControl();

public:
	bbaTasksUtils();
	virtual ~bbaTasksUtils();
	void applyTasks();
};

#endif /* UTILS_BBATASKSUTILS_H_ */
