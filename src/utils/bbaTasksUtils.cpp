/*
 * bbaTasksUtils.cpp
 *
 *  Created on: 5 Eyl 2023
 *      Author: burak
 */

#include "bbaTasksUtils.h"

#include "symbols/bbaGlobalSymbols.h"

bbaTasksUtils::bbaTasksUtils() {
	timer.attach(100, [this]() {
		applyTasks();
	});
	count = 0;
	wifiLedState = false;
}

bbaTasksUtils::~bbaTasksUtils() {
	timer.detach();
}

void bbaTasksUtils::applyTasks() {
	wifiLedControl();

	count++;
}

void bbaTasksUtils::wifiLedControl() {
	if (bbaGlobalSymbols::isWifiLedBlink()) {
		if (count % 2 == 0) {
			espGpioController.wifiLedToggle();
		}
	} else if (!wifiLedState && bbaGlobalSymbols::isWifiLedOn()) {
		espGpioController.wifiLedOn();
		wifiLedState = true;
	} else {
		espGpioController.wifiLedOff();
		wifiLedState = false;
	}
}
