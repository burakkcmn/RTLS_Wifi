/*
 * bbaLocationPacket.h
 *
 *  Created on: Jan 27, 2022
 *      Author: aress
 */

#ifndef DATAPACKETS_BBALOCATIONPACKET_H_
#define DATAPACKETS_BBALOCATIONPACKET_H_

#include <vector>
#include "bbaPacketBase.h"

#include "../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERIAL_LOCATION_PACKET_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#include "../utils/StringProcesses.h"
#else
#include <Arduino.h>
#include <string>
#endif

using namespace std;

class bbaLocationPacket {
public:
	bbaLocationPacket(const MESSAGESOURCES&, const MESSAGESOURCES&,
			const MESSAGECONTENT&);
	virtual ~bbaLocationPacket();
	void createData(const int &tagid, const string &tagchipid,
			const string &tagbtoothid, const int &anchorid,
			const string &anchorchipid, const string &anchorbtoothid,
			const string &anchormacadr, const float &distance);
	vector<unsigned char> getPacket();

private:
	const char beginChar = 0x23;
	const char endChar = 0x23;

	struct PACKETDATA    //from tag
	{
		int anchor_id;
		string anchor_chip_id;
		string anchor_btooth_id;
		string anchor_mac_adr;
		int tag_id;
		string tag_chip_id;
		string tag_btooth_id;
		float distance;
	};

	MESSAGESOURCES source;                 //Source device name as SOURCES
	MESSAGESOURCES destination;         //destination device name as SOURCES
	MESSAGECONTENT content;				//Message content
	int packetdata_length = 90;
	PACKETDATA packetData;

	vector<unsigned char> intToByte(const int&);
	vector<unsigned char> floatToByte(const float&);
	float byteToFloat(const vector<unsigned char>&);
	vector<unsigned char> stringToByte(const string&);
	uint32_t calculateCRC(const vector<unsigned char>&);
	vector<unsigned char> packetDataToByte(const PACKETDATA &pd);
};

#endif /* DATAPACKETS_BBALOCATIONPACKET_H_ */
