/*
 * bbaEspGpioController.cpp
 *
 *  Created on: 5 Eyl 2023
 *      Author: burak
 */

#include "bbaEspGpioController.h"

#include "../../utils/symbols/bbaGlobalSymbols.h"

bbaEspGpioController::bbaEspGpioController() {
	// TODO Auto-generated constructor stub

}

bbaEspGpioController::~bbaEspGpioController() {
	// TODO Auto-generated destructor stub
}

bool bbaEspGpioController::getPinState(uint8_t pin) {
	return digitalRead(pin) == 0;
}

void bbaEspGpioController::setPinState(uint8_t pin, bool state) {
	digitalWrite(pin, state);
}

void bbaEspGpioController::togglePinState(uint8_t pin) {
	digitalWrite(pin, getPinState(pin) ^ 1);
}

void bbaEspGpioController::wifiLedToggle() {
	togglePinState(bbaGlobalSymbols::getWifiLedPin());
}

void bbaEspGpioController::wifiLedOn() {
	setPinState(bbaGlobalSymbols::getWifiLedPin(), true);
}

void bbaEspGpioController::wifiLedOff() {
	setPinState(bbaGlobalSymbols::getWifiLedPin(), false);
}
