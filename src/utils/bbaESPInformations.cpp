/*
 * bbaESPInformations.cpp
 *
 *  Created on: Feb 22, 2023
 *      Author: aress
 */

#include "bbaESPInformations.h"
#include <Arduino.h>

extern "C" {
#include <user_interface.h>
}

#if ISDEFINED(SERIAL_MAIN_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#endif

bbaESPInformations::bbaESPInformations() {
	// TODO Auto-generated constructor stub

}

bbaESPInformations::~bbaESPInformations() {
	// TODO Auto-generated destructor stub
}

std::string bbaESPInformations::getResetInformation() {
	rst_info *resetInfo;
	resetInfo = ESP.getResetInfoPtr();
	ESP.getResetReason();
	std::string reason = "";
	switch ((*resetInfo).reason) {
	case rst_reason::REASON_DEFAULT_RST:
		reason = "REASON_DEFAULT_RST";
		break;

	case rst_reason::REASON_WDT_RST:
		reason = "REASON_WDT_RST";
		break;

	case rst_reason::REASON_EXCEPTION_RST:
		reason = "REASON_EXCEPTION_RST";
		break;

	case rst_reason::REASON_SOFT_WDT_RST:
		reason = "REASON_SOFT_WDT_RST";
		break;

	case rst_reason::REASON_SOFT_RESTART:
		reason = "REASON_SOFT_RESTART";
		break;

	case rst_reason::REASON_DEEP_SLEEP_AWAKE:
		reason = "REASON_DEEP_SLEEP_AWAKE";
		break;

	case rst_reason::REASON_EXT_SYS_RST:
		reason = "REASON_EXT_SYS_RST";
		break;
	}
	return reason;
}

std::string bbaESPInformations::getResetReason() {
	String tmp = ESP.getResetReason();
	std::string reason(tmp.c_str(), tmp.length());
	return reason;
}

std::string bbaESPInformations::getResetInfo() {
	String tmp = ESP.getResetInfo();
	std::string reason(tmp.c_str(), tmp.length());
	return reason;
}

std::string bbaESPInformations::getInformation() {
	rst_info *resetInfo;
	resetInfo = ESP.getResetInfoPtr();
//	std::string reason(tmp.c_str(), tmp.length());
	std::string reason = "";
	return reason;
}
