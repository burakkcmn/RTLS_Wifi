/*
 * bbaDebugPacket.h
 *
 *  Created on: Jan 28, 2022
 *      Author: aress
 */

#ifndef DATAPACKETS_BBADEBUGPACKET_H_
#define DATAPACKETS_BBADEBUGPACKET_H_

#include "bbaPacketBase.h"
#include <vector>
#include <FastCRC.h>

#include "../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERIAL_LOG_PACKET_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#else
#include <Arduino.h>
#include <string>
#endif

using namespace std;

class bbaDebugPacket {
public:
	bbaDebugPacket(const MESSAGECONTENT&);
	virtual ~bbaDebugPacket();
	void createData(const int &anchorid, const string &anchorchipid,
			const std::string &anchorbtoothid, const std::string &anchormacadr,
			const std::string &message);
	vector<unsigned char> getPacket();

private:
	char beginChar = 0x23;
	char endChar = 0x23;

	struct PACKETDATA    //from tag
	{
		int anchor_id;
		string anchor_chip_id;
		std::string anchor_btooth_id;
		std::string anchor_mac_adr;
		int messageLength;
		std::string message;
	};

	MESSAGESOURCES source;                 //Source device name as SOURCES
	MESSAGESOURCES destination;         //destination device name as SOURCES
	MESSAGECONTENT content;
	int packetdata_length = 44;
	PACKETDATA packetData;

	vector<unsigned char> intToByte(const int&);
	vector<unsigned char> stringToByte(const String&);
	vector<unsigned char> stringToByte(const std::string&);
	uint32_t calculateCRC(const vector<unsigned char>&);
	vector<unsigned char> packetDataToByte(const PACKETDATA &pd);
};

#endif /* DATAPACKETS_BBADEBUGPACKET_H_ */
