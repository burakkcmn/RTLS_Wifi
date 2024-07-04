/*
 * bbaLogSymbols.cpp
 *
 *  Created on: Feb 18, 2023
 *      Author: aress
 */

#include "bbaLogSymbols.h"

bool bbaLogSymbols::gpioControllerLog = true;
bool bbaLogSymbols::eepromLog = true;
bool bbaLogSymbols::wifiLog = true;
bool bbaLogSymbols::updateManagerLog = true;
bool bbaLogSymbols::webServiceLog = true;
bool bbaLogSymbols::heartbeatLog = true;
bool bbaLogSymbols::messageBufferLog = true;
bool bbaLogSymbols::locationPacketLog = true;
bool bbaLogSymbols::logPacketLog = true;
bool bbaLogSymbols::registrationPacketLog = true;
bool bbaLogSymbols::serialHandlerLog = true;
bool bbaLogSymbols::mainLog = true;
bool bbaLogSymbols::stringProcessLog = true;
bool bbaLogSymbols::serverCommandHandlerLog = true;
bool bbaLogSymbols::nrf52xxBootloaderLog = true;

bool bbaLogSymbols::getGpioControllerLog() {
	return gpioControllerLog;
}

bool bbaLogSymbols::getEepromLog() {
	return eepromLog;
}

bool bbaLogSymbols::getWifiLog() {
	return wifiLog;
}

bool bbaLogSymbols::getUpdateManagerLog() {
	return updateManagerLog;
}

bool bbaLogSymbols::getWebServiceLog() {
	return webServiceLog;
}

bool bbaLogSymbols::getHeartbeatLog() {
	return heartbeatLog;
}

bool bbaLogSymbols::getMessageBufferLog() {
	return messageBufferLog;
}

bool bbaLogSymbols::getLocationPacketLog() {
	return locationPacketLog;
}

bool bbaLogSymbols::getLogPacketLog() {
	return logPacketLog;
}

bool bbaLogSymbols::getSerialHandlerLog() {
	return serialHandlerLog;
}

bool bbaLogSymbols::getRegistrationPacketLog() {
	return registrationPacketLog;
}

bool bbaLogSymbols::getMainLog() {
	return mainLog;
}

bool bbaLogSymbols::getStringProcessLog() {
	return stringProcessLog;
}

bool bbaLogSymbols::getServerCommandHandlerLog() {
	return serverCommandHandlerLog;
}

bool bbaLogSymbols::getNrf52xxBootloaderLog() {
	return nrf52xxBootloaderLog;
}

