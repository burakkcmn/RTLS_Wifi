/*
 * bbaSerialHandler.cpp
 *
 *  Created on: May 8, 2022
 *      Author: aress
 */

#include "bbaSerialHandler.h"
#include "bbaSerial.h"
#include <vector>
#include <string>
#include "../../utils/StringProcesses.h"
#include "../../wirelessCommunication/bbaWifiManager.h"
#include "../../DataPackets/bbaPacketBase.h"
#include "../../utils/symbols/bbaGlobalSymbols.h"
#include "../../wiredCommunication/serial/dwmCommunications/bbaDWMMessenger.h"

#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
#include "../../utils/bbaServerLog.h"
#include "../../wirelessCommunication/bbaWifiManager.h"
#endif

using namespace std;

bbaSerialHandler::bbaSerialHandler() {
	bbaSerial::setup(115200);
}

bbaSerialHandler::~bbaSerialHandler() {
}

void bbaSerialHandler::process() {
	processRead();
	processWrite();
}

void bbaSerialHandler::processRead() {
	string buffer = bbaSerial::read();
	if (buffer.length() > 0) {
		MessageBuffer.add(buffer);
		string message;
		do {		// while (MessageBuffer.getLength() > 0)
			message = MessageBuffer.getMessage();

			if (isMessageEmpty(&message)) {
				return;
			} else {
				StringProcesses::removeBeginAndEndChars(message);

				vector<string> token = splitTokens(&message);

				if (isLocationTag(&token[0])) {
					int tokenSize = 6;
					if (!isTokenSizeValid(&token, &tokenSize)) {
						return;
					}

					int tagId = 1;
					string tagChipId = getTagChipId(&token[2]);
					StringProcesses::toUpperCase(tagChipId);
					if (tagChipId.empty()) {
						return;
					}

					string tagBtoothChipId = "0000000000000002";

					float distance = getDistance(&token[3]);
					if (distance == -1) {
						return;
					}

					sendLocationInfo(&tagId, &tagChipId, &tagBtoothChipId,
							&distance);

				} else if (isErrorTag(&token[0])) {

					sendErrorLog(&token[1]);

				} else if (isInfoTag(&token[0])) {

					sendInfoLog(&token[1]);

				} else if (isRegisterTag(&token[0])) {

					if (!getAnchorChipId(&token[1])) {
						return;
					}

					if (!getAnchorBluetoothId(&token[2])) {
						return;
					}

					if (!getDWMVersion(&token[3])) {
						return;
					}
					if (!bbaGlobalSymbols::isDwmRegistered()) {
						if (sendRegisterInfo()) {
							bbaDWMMessenger::startMesureRequest();
							bbaGlobalSymbols::setDwmRegistered(true);
						}
					}
				} else if (isRestartTag(&token[0])) {
					ESP.restart();
				} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
					bbaSerial::dbg_printf(
							"[Serial Handler] Message not valid: %s\n",
							message.c_str());
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
					if (bbaLogSymbols::getSerialHandlerLog()
							&& bbaWifiManager::isConnected()) {
						bbaServerLog::error_log(
								"[Serial Handler] Message not valid: %s",
								message.c_str());
					}
#endif
				}
			}

		} while (MessageBuffer.getLength() > 0);
	}
}

void bbaSerialHandler::processWrite() {
	if (!bbaGlobalSymbols::isDwmRegistered() && myTimer.check(5000)) {
		bbaDWMMessenger::registrationRequest();
	}
}

bool bbaSerialHandler::isMessageEmpty(const string *message) {
	if (message == nullptr || message->empty()) {
		return true;
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] Message is empty: %s\n",
				(message != nullptr ? message->c_str() : "null"));
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Serial Handler] Message is empty: %s",
					(message != nullptr ? message->c_str() : "null"));
		}
#endif
	}
	return false;
}

vector<string> bbaSerialHandler::splitTokens(const string *message) {
	vector<string> token;

	if (message != nullptr) {
		token = StringProcesses::split(*message,
				bbaGlobalSymbols::getGlobalSeperator());
	}

#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
	if (message != nullptr) {
		string printStr;
		for (const auto &value : token) {
			printStr += "\tValue: " + value + "\n";
		}
		printStr.erase(printStr.length() - 1);
		bbaSerial::dbg_printf("[Serial Handler] Tokens: \n%s",
				printStr.c_str());
	}
#endif

#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
	if (bbaLogSymbols::getSerialHandlerLog() && message != nullptr
			&& bbaWifiManager::isConnected()) {
		string printStr;
		for (const auto &value : token) {
			printStr += "\tValue: " + value + "\n";
		}
		printStr.erase(printStr.length() - 1);
		bbaServerLog::dbg_log("[Serial Handler] Tokens: \n%s",
				printStr.c_str());
	}
#endif

	return token;
}

bool bbaSerialHandler::isLocationTag(const string *tag) {
	if (tag != nullptr
			&& StringProcesses::contains(*tag,
					bbaGlobalSymbols::getLocationTag())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] Location found...");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Serial Handler] Location found...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaSerialHandler::isTokenSizeValid(const vector<string> *token,
		const int *size) {
	if (token == nullptr || size == nullptr || token->size() < *size) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error. Token size not valid.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Serial Handler] Error. Token size not valid.");
		}
#endif
		return false;
	}
	return true;
}

string bbaSerialHandler::getTagChipId(const string *token) {
	if (token == nullptr
			|| !StringProcesses::contains(*token,
					bbaGlobalSymbols::getGlobalConnector())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error. Tag chip id is not valid. TagId: %s\n",
				(token != nullptr ? token->c_str() : "null"));
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Serial Handler] Error. Tag chip id is not valid. TagId: %s",
					(token != nullptr ? token->c_str() : "null"));
		}
#endif
		return std::string();
	}

	vector<string> tagChipIdKeyValue = StringProcesses::split(*token,
			bbaGlobalSymbols::getGlobalConnector());

#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
	bbaSerial::dbg_printf("[Serial Handler] TagId length: %d\n",
			tagChipIdKeyValue.size());
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
	if (bbaLogSymbols::getSerialHandlerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log("[Serial Handler] TagId length: %d",
				tagChipIdKeyValue.size());
	}
#endif

	if (tagChipIdKeyValue.size() != 2) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error. Tag chip id is not valid. TagId: %s\n",
				(token != nullptr ? token->c_str() : "null"));
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog() && token != nullptr
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Serial Handler] Error. Tag chip id is not valid. TagId: %s",
					(token != nullptr ? token->c_str() : "null"));
		}
#endif
		return std::string();
	}

	string tagChipId = tagChipIdKeyValue[1];

	if (!StringProcesses::checkHex(tagChipId) || tagChipId.length() != 16) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error. Tag chip id is not valid. TagId: %s\n",
				tagChipId.c_str());
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Serial Handler] Error. Tag chip id is not valid. TagId: %s",
					tagChipId.c_str());
		}
#endif
		return std::string();
	}

	return tagChipId;
}

float bbaSerialHandler::getDistance(const string *token) {
	float distance = -1;
	if (token != nullptr
			&& StringProcesses::contains(*token,
					bbaGlobalSymbols::getGlobalConnector())) {
		vector<string> splited = StringProcesses::split(*token,
				bbaGlobalSymbols::getGlobalConnector());
		if (!splited.empty()) {
			if (StringProcesses::isNumber(splited[1])) {
				distance = StringProcesses::convertFloat(splited[1]);
			} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
				bbaSerial::dbg_printf(
						"[Serial Handler] Error. Distance is not a number.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
				if (bbaLogSymbols::getSerialHandlerLog()
						&& bbaWifiManager::isConnected()) {
					bbaServerLog::error_log(
							"[Serial Handler] Error. Distance is not a number.");
				}
#endif
			}
		} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
			bbaSerial::dbg_printf(
					"[Serial Handler] Error. Distance is empty.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
			if (bbaLogSymbols::getSerialHandlerLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::error_log(
						"[Serial Handler] Error. Distance is empty.");
			}
#endif
		}
	} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error. Distance is not valid.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Serial Handler] Error. Distance is not valid.");
		}
#endif
		return -1;
	}

	if (distance < 0 || distance > 6000) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error. Distance is not within limits. Distance: %.0f\n",
				distance);
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Serial Handler] Error. Distance is not within limits. Distance: %.0f",
					distance);
		}
#endif
	}
	return distance;
}

bool bbaSerialHandler::sendLocationInfo(const int *tag_id,
		const string *tag_chip_id, const string *tag_btooth_id,
		const float *distance) {
	if (tag_id == nullptr || tag_chip_id == nullptr || tag_btooth_id == nullptr
			|| distance == nullptr) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error. Invalid tag_id, tag_chip_id, tag_btooth_id, or distance.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Serial Handler] Error. Invalid tag_id, tag_chip_id, tag_btooth_id, or distance.");
		}
#endif
		return false;
	}

	if (serverCommunication.sendLocation(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(), bbaWifiManager::getMac(),
			*tag_id, *tag_chip_id, *tag_btooth_id, *distance)) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] anchor:%s, tag:%s, distance: %.0f		/	done.\n",
				bbaGlobalSymbols::getAnchorChipId().c_str(),
				tag_chip_id->c_str(), *distance);
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log(
					"[Serial Handler] anchor:%s, tag:%s, distance: %.0f		/	done.",
					bbaGlobalSymbols::getAnchorChipId().c_str(),
					tag_chip_id->c_str(), *distance);
		}
#endif
	} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] sending failed.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[Serial Handler] sending failed.");
		}
#endif
		return false;
	}
	return true;
}

bool bbaSerialHandler::isErrorTag(const string *tag) {
	if (tag != nullptr
			&& StringProcesses::contains(*tag,
					bbaGlobalSymbols::getErrorlogTag())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf(
				"[Serial Handler] Error log found.  Sending...\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log(
					"[Serial Handler] Error log found.  Sending...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaSerialHandler::sendErrorLog(const string *message) {
	if (message == nullptr) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] Error. Invalid message.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[Serial Handler] Error. Invalid message.");
		}
#endif
		return false;
	}

	if (serverCommunication.sendLog(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(),
			bbaWifiManager::getMac().c_str(), MESSAGECONTENT::ERROR,
			*message)) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] 		/	done.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Serial Handler] 		/	done.");
		}
#endif
	} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] sending failed.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[Serial Handler] sending failed.");
		}
#endif
		return false;
	}
	return true;
}

bool bbaSerialHandler::isInfoTag(const string *tag) {
	if (tag != nullptr
			&& StringProcesses::contains(*tag,
					bbaGlobalSymbols::getInfoLogTag())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] Info log found.  Sending...\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log(
					"[Serial Handler] Info log found.  Sending...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaSerialHandler::sendInfoLog(const string *message) {
	if (message == nullptr) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] Error. Invalid message.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[Serial Handler] Error. Invalid message.");
		}
#endif
		return false;
	}

	if (serverCommunication.sendLog(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(),
			bbaWifiManager::getMac().c_str(), MESSAGECONTENT::INFORMATION,
			*message)) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] 		/	done.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Serial Handler] 		/	done.");
		}
#endif
	} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] sending failed.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[Serial Handler] sending failed.");
		}
#endif
		return false;
	}
	return true;
}

bool bbaSerialHandler::isRegisterTag(const string *tag) {
	if (tag != nullptr
			&& StringProcesses::contains(*tag,
					bbaGlobalSymbols::getRegisterInfoTag())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] Register Info found...\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Serial Handler] Register Info found...");
		}
#endif
		return true;
	}
	return false;
}

bool bbaSerialHandler::isRestartTag(const string *tag) {
	if (tag == nullptr) {
		return false;
	}

	if (StringProcesses::contains(*tag, bbaGlobalSymbols::getRestartTag())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] Reset request...\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Serial Handler] Reset request...");
		}
#endif
		return true;
	}

	return false;
}

bool bbaSerialHandler::getAnchorChipId(const string *token) {
	if (token == nullptr) {
		return false;
	}

	vector<string> AnchorDwmchipId = StringProcesses::split(*token,
			bbaGlobalSymbols::getGlobalConnector());
	if (AnchorDwmchipId.size() >= 2
			&& StringProcesses::contains(AnchorDwmchipId[0],
					bbaGlobalSymbols::getChipIdTag())) {
		if (!StringProcesses::checkHex(AnchorDwmchipId[1])) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
			bbaSerial::dbg_printf(
					"[Serial Handler] Error. Anchor chip id is not valid. Anchor chip id: %s\n",
					AnchorDwmchipId[1].c_str());
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
			if (bbaLogSymbols::getSerialHandlerLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::error_log(
						"[Serial Handler] Error. Anchor chip id is not valid. Anchor chip id: %s",
						AnchorDwmchipId[1].c_str());
			}
#endif
			return false;
		}
		StringProcesses::toUpperCase(AnchorDwmchipId[1]);
		bbaGlobalSymbols::setAnchorChipId(AnchorDwmchipId[1]);
		return true;
	}

	return false;
}

bool bbaSerialHandler::getAnchorBluetoothId(const string *token) {
	if (token == nullptr) {
		return false;
	}

	vector<string> AnchorDwmBluetoothId = StringProcesses::split(*token,
			bbaGlobalSymbols::getGlobalConnector());
	if (AnchorDwmBluetoothId.size() >= 2
			&& StringProcesses::contains(AnchorDwmBluetoothId[0],
					bbaGlobalSymbols::getBluetoothIdTag())) {
		StringProcesses::toUpperCase(AnchorDwmBluetoothId[1]);
		bbaGlobalSymbols::setAnchorBluetoothId(AnchorDwmBluetoothId[1]);
		if (!StringProcesses::checkHex(
				bbaGlobalSymbols::getAnchorBluetoothId())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
			bbaSerial::dbg_printf(
					"[Serial Handler] Error. Tag bluetooth id is not valid(HEX). bluetooth id: %s\n",
					bbaGlobalSymbols::getAnchorBluetoothId().c_str());
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
			if (bbaLogSymbols::getSerialHandlerLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::error_log(
						"[Serial Handler] Error. Tag bluetooth chip id is not valid(HEX). bluetooth id: %s",
						bbaGlobalSymbols::getAnchorBluetoothId().c_str());
			}
#endif
			return false;
		}
		return true;
	}

	return false;
}

bool bbaSerialHandler::getDWMVersion(const string *token) {
	if (token == nullptr) {
		return false;
	}

	vector<string> AnchorDwmVersion = StringProcesses::split(*token,
			bbaGlobalSymbols::getGlobalConnector());
	if (AnchorDwmVersion.size() >= 2
			&& StringProcesses::contains(AnchorDwmVersion[0],
					bbaGlobalSymbols::getVersionTag())) {
		bbaGlobalSymbols::setAnchorDwmVersion(AnchorDwmVersion[1]);
		return true;
	}

	return false;
}

bool bbaSerialHandler::sendRegisterInfo() {
	if (serverCommunication.registration(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(),
			bbaWifiManager::getMac().c_str(), bbaGlobalSymbols::getVersion(),
			bbaGlobalSymbols::getAnchorDwmVersion())) {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] 		/	done.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Serial Handler] 		/	done.");
		}
#endif
	} else {
#if ISDEFINED(SERIAL_SERIAL_HANDLER_LOG)
		bbaSerial::dbg_printf("[Serial Handler] sending failed.\n");
#endif
#if ISDEFINED(SERVER_SERIAL_HANDLER_LOG)
		if (bbaLogSymbols::getSerialHandlerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[Serial Handler] sending failed.");
		}
#endif
		return false;
	}
	return true;
}
