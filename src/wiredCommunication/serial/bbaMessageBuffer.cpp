/*
 * bbaMessage.cpp
 *
 *  Created on: Feb 20, 2022
 *      Author: aress
 */

#include "bbaMessageBuffer.h"

#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
#include "../../utils/bbaServerLog.h"
#include "../../utils/StringProcesses.h"
#include "../../utils/convertUtil.h"
#include "../../wirelessCommunication/bbaWifiManager.h"
#endif

bbaMessageBuffer::bbaMessageBuffer() {
}

bbaMessageBuffer::~bbaMessageBuffer() {
}

void bbaMessageBuffer::add(const std::string &msg) {
	buffer.append(msg);

#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
	bbaSerial::dbg_printf("[Message Buffer] Add: %s\n New Buffer: %s \n",
			msg.c_str(), buffer.c_str());
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
	if (bbaLogSymbols::getMessageBufferLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log("[Message Buffer] Add: %s\nNew Buffer: %s",
				msg.c_str(), buffer.c_str());
	}
#endif
}

std::string bbaMessageBuffer::getMessage() {
	return parsMessage();
}

int bbaMessageBuffer::getLength() {
	return buffer.length();
}

string bbaMessageBuffer::getBuffer() {
	return buffer;
}

std::string bbaMessageBuffer::parsMessage() {
	if (buffer.length() > 0) {
		int beginIndex = -1;
		int endIndex = -1;
		for (int i = 0; i < buffer.length(); i++) {

			if (compareWithBeginChar(buffer.at(i), i)) {

				if (isBeginIndexAlreadyFound(beginIndex, i)) { //Begin index found again before end index
					deleteRangeFromBuffer(0, i);
					return std::string();
				}		// if (beginIndex != -1)

				beginIndex = i;
			} else if (compareWithEndChar(buffer.at(i), i)) {//end of if (buffer.at(i) == messageBegin)		//end index found

				if (!isBeginIndexFound(beginIndex, i)) {//bug begin index not found
					deleteRangeFromBuffer(0, i + 1);	//delete till end index
					return std::string();
				} else {			// if (beginIndex == -1)
					endIndex = i;
					break;
				}		// if (beginIndex == -1) else

			}		// else if (buffer.at(i) == messageEnd)

		}	//	for (int i = 0; i < buffer.length(); i++)
		if (beginIndex != -1 && endIndex != -1) {
			string message = createMessage(beginIndex, endIndex + 1);

			deleteRangeFromBuffer(0, endIndex + 1);

			return message;
		}		// if (beginIndex != -1 && endIndex != -1)
	}		// if (buffer.length() > 0)
	return std::string();
}

bool bbaMessageBuffer::compareWithBeginChar(const char &chr, int &beginIndex) {
	if (chr == messageBegin) {
#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
		bbaSerial::dbg_printf("[Message Buffer] Begin index found at: %d\n",
				beginIndex);
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
		if (bbaLogSymbols::getMessageBufferLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Message Buffer] Begin index found at: %d",
					beginIndex);
		}
#endif
		return true;
	}
	return false;
}

bool bbaMessageBuffer::compareWithEndChar(const char &chr, int &endIndex) {
	if (chr == messageEnd) {
#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
		bbaSerial::dbg_printf("[Message Buffer] End index found at: %d\n",
				endIndex);
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
		if (bbaLogSymbols::getMessageBufferLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[Message Buffer] End index found at: %d",
					endIndex);
		}
#endif
		return true;
	}
	return false;
}

bool bbaMessageBuffer::isBeginIndexAlreadyFound(const int &beginIndex,
		const int &index) {
	if (beginIndex != -1) { // Begin index found again before end index
#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
		bbaSerial::dbg_printf(
				"[Message Buffer] Begin char find second time.\nbuffer before: %s \n[begin char first index: %d and second index: %d]\n",
				buffer.c_str(), beginIndex, index);
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
		if (bbaLogSymbols::getMessageBufferLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Message Buffer] Begin char found second time. Buffer before: %s [begin char first index: %d and second index: %d]",
					buffer.c_str(), beginIndex, index);
		}
#endif
		return true;
	}
	return false;
}

bool bbaMessageBuffer::isBeginIndexFound(const int &beginIndex,
		const int &index) {
	if (beginIndex == -1) { // Begin index found again before end index
#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
		bbaSerial::dbg_printf(
				"[Message Buffer] Message start with end index(%d). Message deleted from buffer.\nNew Buffer: %s\n",
				index, buffer.c_str());
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
		if (bbaLogSymbols::getMessageBufferLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[Message Buffer] Message start with end index(%d). Message deleted from buffer.\nNew Buffer: %s",
					index, buffer.c_str());
		}
#endif
		return false;
	}
	return true;
}

void bbaMessageBuffer::deleteRangeFromBuffer(const int &startIndex,
		const int &endIndex) {
	if (startIndex >= 0 //
	&& startIndex < endIndex //
	&& endIndex <= buffer.length()) {
		std::string newBuffer = buffer.substr(0, startIndex)
				+ buffer.substr(endIndex);
		buffer = newBuffer;
	} else {
//		buffer = "";
#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
		bbaSerial::dbg_printf(
				"[Message Buffer] Wrong Delete Range: (startindex: %d, endindex: %d)\n",
				startIndex, endIndex);
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
		if (bbaLogSymbols::getMessageBufferLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log(
					"[Message Buffer] Wrong Delete Range: (startindex: %d, endindex: %d)",
					startIndex, endIndex);
		}
#endif
	}

#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
	bbaSerial::dbg_printf("[Message Buffer] Erase from buffer: %s\n",
			buffer.c_str());
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
	if (bbaLogSymbols::getMessageBufferLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log("[Message Buffer] Erase from buffer:%s",
				buffer.c_str());
	}
#endif
}

string bbaMessageBuffer::createMessage(const int &startIndex,
		const int &endIndex) {
	string message = buffer.substr(startIndex, endIndex);
#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
	bbaSerial::dbg_printf("[Message Buffer] Message created for process: %s\n",
			message.c_str());
#endif
#if ISDEFINED(SERVER_MESSAGEBUFFER_LOG)
	if (bbaLogSymbols::getMessageBufferLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[Message Buffer] Message created for process: %s",
				message.c_str());
	}
#endif
	return message;
}

