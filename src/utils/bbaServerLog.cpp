/*
 * bbadebug.cpp
 *
 *  Created on: Jan 16, 2022
 *      Author: aress
 */

#include "bbaServerLog.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#include "../ServerCommunications/bbaServerCommunicationHandler.h"
#include "../DataPackets/bbaPacketBase.h"
#include "symbols/bbaGlobalSymbols.h"

#include "../wiredCommunication/serial/bbaSerial.h"

static bbaServerCommunicationHandler serverCommunication;

void bbaServerLog::dbg_log(const char *format, ...) {
	constexpr size_t MaxBufferSize = 1400;
	char sbuf[MaxBufferSize];
	va_list varArgs;

	va_start(varArgs, format);
	vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
	va_end(varArgs);

	serverCommunication.sendLog(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(), bbaWifiManager::getMac(),
			MESSAGECONTENT::DEBUG, sbuf);
}

void bbaServerLog::info_log(const char *format, ...) {
	constexpr size_t MaxBufferSize = 1400;
	char sbuf[MaxBufferSize];
	va_list varArgs;

	va_start(varArgs, format);
	vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
	va_end(varArgs);

	serverCommunication.sendLog(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(), bbaWifiManager::getMac(),
			MESSAGECONTENT::INFORMATION, sbuf);
}

void bbaServerLog::error_log(const char *format, ...) {
	constexpr size_t MaxBufferSize = 1400;
	char sbuf[MaxBufferSize];
	va_list varArgs;

	va_start(varArgs, format);
	vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
	va_end(varArgs);

	serverCommunication.sendLog(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(), bbaWifiManager::getMac(),
			MESSAGECONTENT::ERROR, sbuf);
}

void bbaServerLog::warning_log(const char *format, ...) {
	constexpr size_t MaxBufferSize = 1400;
	char sbuf[MaxBufferSize];
	va_list varArgs;

	va_start(varArgs, format);
	vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
	va_end(varArgs);

	serverCommunication.sendLog(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(), bbaWifiManager::getMac(),
			MESSAGECONTENT::WARNING, sbuf);
}

void bbaServerLog::send_log(const char *format, ...) {
	constexpr size_t MaxBufferSize = 1400;
	char sbuf[MaxBufferSize];
	va_list varArgs;

	va_start(varArgs, format);
	vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
	va_end(varArgs);

	serverCommunication.sendLog(bbaGlobalSymbols::getAnchorId(),
			bbaGlobalSymbols::getAnchorChipId(),
			bbaGlobalSymbols::getAnchorBluetoothId(), bbaWifiManager::getMac(),
			MESSAGECONTENT::WARNING, sbuf);
}
