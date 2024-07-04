/*
 * eepromUtil.cpp
 *
 *  Created on: Jan 1, 2023
 *      Author: aress
 */

#include "eepromUtil.h"
#include "convertUtil.h"

#include "symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERVER_EEPROM_LOG)
#include "../../utils/bbaServerLog.h"
#include "StringProcesses.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#endif

void eepromUtil::begin() {
	EEPROM.begin(1000);
}

bool eepromUtil::end() {
	return EEPROM.end();
}

bool eepromUtil::write(const int &address, const uint8_t *data,
		const uint8_t &length) {
#if ISDEFINED(SERVER_EEPROM_LOG)
	string logString;
	if (bbaLogSymbols::getEepromLog()) {
		logString = "Write byte: ";
	}
#endif

	for (int i = 0; i < length; i++) {
		EEPROM.write(address + i, data[i]);

#if ISDEFINED(SERVER_EEPROM_LOG)
		if (bbaLogSymbols::getEepromLog()) {
			logString.append(StringProcesses::create("%d,", data[i]));
		}
#endif

	}

#if ISDEFINED(SERVER_EEPROM_LOG)
	if (bbaLogSymbols::getEepromLog() && bbaWifiManager::isConnected()) {
		logString.append("\n");
		bbaServerLog::dbg_log("[EEPROM] %s", logString.c_str());
	}
#endif

	return EEPROM.commit();
}

uint8_t* eepromUtil::read(const int &address, const uint8_t &length) {
	uint8_t *byteData = new uint8_t[length];
#if ISDEFINED(SERVER_EEPROM_LOG)
	string logString;
	if (bbaLogSymbols::getEepromLog()) {
		logString = "Read byte: ";
	}
#endif

	for (int i = 0; i < length; i++) {
		byteData[i] = EEPROM.read(address + i);

#if ISDEFINED(SERVER_EEPROM_LOG)
		if (bbaLogSymbols::getEepromLog() && bbaWifiManager::isConnected()) {
			logString.append(StringProcesses::create("%d,", byteData[i]));
		}
#endif

	}

#if ISDEFINED(SERVER_EEPROM_LOG)
	if (bbaLogSymbols::getEepromLog() && bbaWifiManager::isConnected()) {
		logString.append("\n");
		bbaServerLog::dbg_log("[EEPROM] %s", logString.c_str());
	}
#endif

	return byteData;
}

bool eepromUtil::write(const int &address, const string &data,
		const uint8_t &length) {
	uint8_t *byteData = (uint8_t*) malloc(length);
	memset(byteData, 0, length);

	memcpy(byteData, convertUtil::stringToByteArray(data), data.length());

	return write(address, byteData, length);
}

string eepromUtil::readString(const int &address, const uint8_t &length) {
	uint8_t *byteData = read(address, length + 1);
	return convertUtil::byteArrayToString(byteData, length);
}

bool eepromUtil::write(const int &address, const int &data) {
	return write(address, convertUtil::intToByteArray(data), 4);
}

int eepromUtil::readInt(const int &address) {
	return convertUtil::byteArrayToInt(read(address, 4));
}

bool eepromUtil::write(const int &address, const uint8_t &bit,
		const bool &data) {
	uint8_t *tmpdata = read(address, 1);
//	bbaSerial::dbg_printf("bool data: %d\n", tmpdata[0]);
	uint8_t writedata = tmpdata[0];
	if (data) {
		writedata |= 1UL << bit;
	} else {
		writedata &= ~(1UL << bit);
	}
//	bbaSerial::dbg_printf("write data: %d\n", writedata);
	return write(address, &writedata, 1);
}

bool eepromUtil::readbool(const int &address, const uint8_t &bit) {
	return (((read(address, 1)[0] >> bit) & 1U) == 1 ? true : false);
}

bool eepromUtil::write(const int &address, const float &data) {
	return true;
}

float eepromUtil::readFloat(const int &address) {
	return 1.1;
}
