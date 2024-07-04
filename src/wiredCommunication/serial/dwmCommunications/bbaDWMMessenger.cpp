/*
 * bbaDWMMessenger.cpp
 *
 *  Created on: May 16, 2022
 *      Author: aress
 */

#include "bbaDWMMessenger.h"
#include "../../../utils/symbols/bbaGlobalSymbols.h"

void bbaDWMMessenger::registrationRequest() {
	bbaSerial::dbg_printf("%s",
			bbaGlobalSymbols::getRegistrationRequest().c_str());
}

void bbaDWMMessenger::startMesureRequest() {
	bbaSerial::dbg_printf("%s",
			bbaGlobalSymbols::getStartMesurerequest().c_str());
}

void bbaDWMMessenger::getLastErrorMessageRequest() {
	bbaSerial::dbg_printf("%s",
			bbaGlobalSymbols::getRegistrationRequest().c_str());
}
