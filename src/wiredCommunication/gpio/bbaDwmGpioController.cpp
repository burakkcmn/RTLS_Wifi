/*
 * bbaDwmGpioController.cpp
 *
 *  Created on: Feb 14, 2023
 *      Author: aress
 */

#include "bbaDwmGpioController.h"

#include "../../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERVER_GPIO_LOG)
#include "../../utils/bbaServerLog.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#endif

#if ISDEFINED(SERIAL_GPIO_LOG)
#include "../serial/bbaSerial.h"
#else
#include <arduino.h>
#endif

bbaDwmGpioController::bbaDwmGpioController() {
}

bbaDwmGpioController::~bbaDwmGpioController() {
	// TODO Auto-generated destructor stub
}

void bbaDwmGpioController::begin() {
	pinMode(bbaGlobalSymbols::getDwmSWRSTPin(), OUTPUT);
	digitalWrite(bbaGlobalSymbols::getDwmSWRSTPin(), LOW);

	pinMode(bbaGlobalSymbols::getDwmSWRSTPin(), OUTPUT);
	digitalWrite(bbaGlobalSymbols::getDwmSWCLKPin(), HIGH);

	pinMode(bbaGlobalSymbols::getDwmSWRSTPin(), OUTPUT);
	digitalWrite(bbaGlobalSymbols::getDwmSWDIOPin(), HIGH);
}

void bbaDwmGpioController::stopDwm() {
	digitalWrite(bbaGlobalSymbols::getDwmSWRSTPin(), LOW);
#if ISDEFINED(SERIAL_MAIN_LOG)
	bbaSerial::dbg_printf("[GPIO] DWM Stopped.\n\n");
#endif
#if ISDEFINED(SERVER_GPIO_LOG)
	if (bbaLogSymbols::getGpioControllerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("[GPIO] DWM Stopped.");
	}
#endif

}

void bbaDwmGpioController::startDwm() {
	digitalWrite(bbaGlobalSymbols::getDwmSWRSTPin(), HIGH);
#if ISDEFINED(SERIAL_MAIN_LOG)
	bbaSerial::dbg_printf("[GPIO] DWM Started.\n\n");
#endif
#if ISDEFINED(SERVER_GPIO_LOG)
	if (bbaLogSymbols::getGpioControllerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("[GPIO] DWM Started.");
	}
#endif

}

void bbaDwmGpioController::resetDwm() {
	digitalWrite(bbaGlobalSymbols::getDwmSWRSTPin(), LOW);
	delayMicroseconds(500);
	digitalWrite(bbaGlobalSymbols::getDwmSWRSTPin(), HIGH);
#if ISDEFINED(SERIAL_MAIN_LOG)
	bbaSerial::dbg_printf("[GPIO] DWM reset action.\n\n");
#endif
#if ISDEFINED(SERVER_GPIO_LOG)
	if (bbaLogSymbols::getGpioControllerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log("[GPIO] DWM reset action.");
	}
#endif

}
