/*
 * bbaRegistrationPacket.h
 *
 *  Created on: Jan 26, 2022
 *      Author: aress
 */

#ifndef DATAPACKETS_BBAREGISTRATIONPACKET_H_
#define DATAPACKETS_BBAREGISTRATIONPACKET_H_

#include <Arduino.h>
#include <vector>
#include <FastCRC.h>
#include "bbaPacketBase.h"

#include "../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERIAL_REGISTRATION_PACKET_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#else
#include <Arduino.h>
#include <string>
#endif

using namespace std;

class bbaRegistrationPacket {
public:
	bbaRegistrationPacket(const MESSAGESOURCES&, const MESSAGESOURCES&,
			const MESSAGECONTENT&);
	virtual ~bbaRegistrationPacket();
	void createData(const int &anchorid, const string &anchorchipid,
			const string &anchorbtoothid, const string &anchormacadr,
			const string &anchorEspVersion, const string &anchorDwmVersion);
	vector<unsigned char> getPacket();

private:
	char beginChar = 0x23;
	char endChar = 0x23;

	struct PACKETDATA    //from tag
	{
		int anchor_id;
		string anchor_chip_id;
		string anchor_btooth_id;
		string anchor_mac_adr;
		string anchorEspVersion;
		string anchorDwmVersion;
		string tagDwmVersion;
	};

	MESSAGESOURCES source;           //Source device name as SOURCES
	MESSAGESOURCES destination; //destination device name as SOURCES
	MESSAGECONTENT content;
	int packetdata_length = 42;
	PACKETDATA packetData;

	vector<unsigned char> intToByte(const int&);
	vector<unsigned char> stringToByte(const string&);
	uint32_t calculateCRC(const vector<unsigned char>&);
	vector<unsigned char> packetDataToByte(const PACKETDATA &pd);
};

#endif /* DATAPACKETS_BBAREGISTRATIONPACKET_H_ */
