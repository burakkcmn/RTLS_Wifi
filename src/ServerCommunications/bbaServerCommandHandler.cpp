/*
 * bbaServerCommandHandler.cpp
 *
 *  Created on: Mar 26, 2023
 *      Author: aress
 */

#include "bbaServerCommandHandler.h"
#include "../utils/StringProcesses.h"
#include "../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
#include "../utils/bbaServerLog.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#endif

#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#endif

bbaServerCommandHandler::bbaServerCommandHandler() {
	serverCommunication.begin();
}

bbaServerCommandHandler::~bbaServerCommandHandler() {
	// TODO Auto-generated destructor stub
}

void bbaServerCommandHandler::process() {
	string packetData = serverCommunication.receive();
	if (packetData.length() > 0) {
		StringProcesses::removeBeginAndEndChars(packetData);

		vector<string> token = splitTokens(packetData);

		if (isRestartTag(token[0])) {
			ESP.restart();
		} else if (isAnchorIdChangeTag(token[0])) {

		} else if (isTagIdChangeTag(token[0])) {

		} else if (isAnchorWifiLedBlinkTag(token[0])) {
			if (StringProcesses::contains(token[1], "true")) {
				bbaGlobalSymbols::setWifiLedBlink(true);
			} else {
				bbaGlobalSymbols::setWifiLedBlink(false);
			}
		} else if (isAnchorWifiLedOnTag(token[0])) {
			if (StringProcesses::contains(token[1], "true")) {
				bbaGlobalSymbols::setWifiLedOn(true);
			} else {
				bbaGlobalSymbols::setWifiLedOn(false);
			}
		} else if (isStopRunningTag(token[0])) {
			if (StringProcesses::contains(token[1], "true")) {
				bbaGlobalSymbols::setIsRunning(true);
			} else {
				bbaGlobalSymbols::setIsRunning(false);
			}
		} else if (isRegisterRequestTag(token[0])) {
			if (StringProcesses::contains(token[1], "true")) {
				bbaGlobalSymbols::setIsRunning(true);
			} else {
				bbaGlobalSymbols::setIsRunning(false);
			}
		} else {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
			bbaSerial::dbg_printf(
					"[Server Command Handler] Message not valid: %s\n\n",
					packetData.c_str());
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
			if (bbaLogSymbols::getServerCommandHandlerLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::error_log(
						"[Server Command Handler] Message not valid: %s",
						packetData.c_str());
			}
#endif
		}
	}
}

vector<string> bbaServerCommandHandler::splitTokens(const string &message) {
	vector<string> token = StringProcesses::split(message,
			bbaGlobalSymbols::getGlobalSeperator());

#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
	string printStr;
	for (int i = 0; i < token.size(); i++) {
		printStr.append("\tValue: ");
		printStr.append(token[i]);
		printStr.append("\n");
	}
	bbaSerial::dbg_printf("[Server Command Handler] Tokens: \n%s",
			printStr.c_str());
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
	if (bbaLogSymbols::getServerCommandHandlerLog()
			&& bbaWifiManager::isConnected()) {
		string printStr;
		for (int i = 0; i < token.size(); i++) {
			printStr.append("\tValue: ");
			printStr.append(token[i]);
			printStr.append("\n");
		}
		bbaServerLog::dbg_log("[Server Command Handler] Tokens: \n%s",
				printStr.c_str());
	}
#endif
	return token;
}

bool bbaServerCommandHandler::isRestartTag(const string &tag) {
	if (StringProcesses::contains(tag, bbaGlobalSymbols::getRestartTag())) {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
		bbaSerial::dbg_printf("[Server Command Handler] Reset request...\n\n");
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
		if (bbaLogSymbols::getServerCommandHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log("[Server Command Handler] Reset request...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaServerCommandHandler::isAnchorIdChangeTag(const string &tag) {
	if (StringProcesses::contains(tag,
			bbaGlobalSymbols::getAnchorIdChangeTag())) {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Server Command Handler] Anchor Id Change Request...\n\n");
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
		if (bbaLogSymbols::getServerCommandHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log(
					"[Server Command Handler] Anchor Id Change Request...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaServerCommandHandler::isTagIdChangeTag(const string &tag) {
	if (StringProcesses::contains(tag, bbaGlobalSymbols::getTagIdChangeTag())) {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Server Command Handler] Tag Id Change Request...\n\n");
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
		if (bbaLogSymbols::getServerCommandHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log(
					"[Server Command Handler] Tag Id Change Request...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaServerCommandHandler::isAnchorWifiLedBlinkTag(const string &tag) {
	if (StringProcesses::contains(tag,
			bbaGlobalSymbols::getAnchorWifiLedBlinkTag())) {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Server Command Handler] Wifi Led Blink Request...\n\n");
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
		if (bbaLogSymbols::getServerCommandHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log(
					"[Server Command Handler] Wifi Led Blink Request...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaServerCommandHandler::isAnchorWifiLedOnTag(const string &tag) {
	if (StringProcesses::contains(tag,
			bbaGlobalSymbols::getAnchorWifiLedOnTag())) {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Server Command Handler] Wifi Led Blink Request...\n\n");
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
		if (bbaLogSymbols::getServerCommandHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log(
					"[Server Command Handler] Wifi Led Blink Request...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaServerCommandHandler::isStopRunningTag(const string &tag) {
	if (StringProcesses::contains(tag,
			bbaGlobalSymbols::getRegisterRequestTag())) {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Server Command Handler] Wifi Led Blink Request...\n\n");
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
		if (bbaLogSymbols::getServerCommandHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log(
					"[Server Command Handler] Wifi Led Blink Request...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaServerCommandHandler::isRegisterRequestTag(const string &tag) {
	if (StringProcesses::contains(tag, bbaGlobalSymbols::getStopRunningTag())) {
#if ISDEFINED(SERIAL_SERVER_COMMANDS_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Server Command Handler] Wifi Led Blink Request...\n\n");
#endif
#if ISDEFINED(SERVER_SERVER_COMMANDS_HANDLER_LOG)
		if (bbaLogSymbols::getServerCommandHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log(
					"[Server Command Handler] Wifi Led Blink Request...");
		}
#endif
		return true;
	}
	return false;
}

