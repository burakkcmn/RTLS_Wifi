/*
 * bbaLocationPacket.cpp
 *
 *  Created on: Jan 27, 2022
 *      Author: aress
 */

#include "bbaLocationPacket.h"
#include <FastCRC.h>

bbaLocationPacket::bbaLocationPacket(const MESSAGESOURCES &msSource,
		const MESSAGESOURCES &msDestination, const MESSAGECONTENT &mcContent) {
	source = msSource;
	destination = msDestination;
	content = mcContent;
}

bbaLocationPacket::~bbaLocationPacket() {
	// TODO Auto-generated destructor stub
}

void bbaLocationPacket::createData(const int &tagid, const string &tagchipid,
		const string &tagbtoothid, const int &anchorid,
		const string &anchorchipid, const string &anchorbtoothid,
		const string &anchormacadr, const float &distance) {
	packetData.anchor_btooth_id = anchorbtoothid;
	packetData.anchor_chip_id = anchorchipid;
	packetData.anchor_id = anchorid;
	packetData.anchor_mac_adr = anchormacadr;
	packetData.tag_btooth_id = tagbtoothid;
	packetData.tag_chip_id = tagchipid;
	packetData.tag_id = tagid;
	packetData.distance = distance;
}

vector<unsigned char> bbaLocationPacket::intToByte(const int &number) {
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (number >> (i * 8));
	return arrayOfByte;
}

vector<unsigned char> bbaLocationPacket::floatToByte(const float &number) {
	// get vector of the right size
	std::vector<unsigned char> data(sizeof(number));
	// copy the bytes
	std::memcpy(data.data(), &number, sizeof(number));
	return data;
}

float bbaLocationPacket::byteToFloat(const vector<unsigned char> &number) {
	float f;
	// make sure the vector is the right size
	if (number.size() != sizeof(f)) {
#if ISDEFINED(SERVER_LOCATION_PACKET_LOG)
		if (bbaLogSymbols::getLocationPacketLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"Byte To Float Error. Size of data in vector and float do not match.");
		}
#endif
#if ISDEFINED(SERIAL_LOCATION_PACKET_LOG)
		bbaSerial::dbg_printf(
				"Byte To Float Error. Size of data in vector and float do not match.\n\n");
#endif
		return -1;
//		throw std::runtime_error{"Size of data in vector and float do not match"}; // copy the bytes into the float
	}
	std::memcpy(&f, number.data(), sizeof(f));
	return f;
}

vector<unsigned char> bbaLocationPacket::stringToByte(const string &str) {
	vector<unsigned char> bytes(str.begin(), str.end());
	bytes.push_back('\0');
	return bytes;
}

vector<unsigned char> bbaLocationPacket::getPacket() {
	vector<unsigned char> packetTobyte;

	vector<unsigned char> tmp = intToByte(source);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	tmp = intToByte(destination);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	tmp = intToByte(content);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	vector<unsigned char> packetdatabyte = packetDataToByte(packetData);

	tmp = intToByte(packetdata_length);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	packetTobyte.insert(packetTobyte.end(), packetdatabyte.begin(),
			packetdatabyte.end());

	uint32_t crc = calculateCRC(packetTobyte);
	tmp = intToByte(crc);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	packetTobyte.insert(packetTobyte.begin(), 1, beginChar);
	packetTobyte.push_back(endChar);

#if ISDEFINED(SERIAL_LOCATION_PACKET_LOG)
	//Print packet...
	bbaSerial::dbg_printf(
			"LOCATION PACKET:\nSource:%d\nDestination:%d\nContent:%d\nTotal Length:%d\n",
			source, destination, content, packetdata_length);

	bbaSerial::dbg_printf(
			"Anchor Id:%d\nAnchor Chip Id:%s\nAnchor Bluetooth Id:%s\nAnchor Mac Adr:%s\n",
			packetData.anchor_id, packetData.anchor_chip_id.c_str(),
			packetData.anchor_btooth_id.c_str(),
			packetData.anchor_mac_adr.c_str());

	bbaSerial::dbg_printf(
			"Tag Id:%d\nTag Chip Id:%s\nTag Bluetooth Id:%s\nDistance:%.0f\n",
			packetData.tag_id, packetData.tag_chip_id.c_str(),
			packetData.tag_btooth_id.c_str(), packetData.distance);
	//Print vector...
	int len = packetTobyte.size();
	string tmpLog = "";
	tmpLog.append(
			StringProcesses::create("LOCATION PACKET: len->%d	packet->", len));
	for (int i = 0; i < len; i++) {
		tmpLog.append(StringProcesses::create("%x - ", packetTobyte[i]));
	}
	bbaSerial::dbg_printf(tmpLog.c_str());
#endif

	return packetTobyte;
}

uint32_t bbaLocationPacket::calculateCRC(const vector<unsigned char> &data) {
	FastCRC32 CRC32;
	return CRC32.crc32(&data[0], data.size());
}

vector<unsigned char> bbaLocationPacket::packetDataToByte(
		const PACKETDATA &pd) {
	vector<unsigned char> packetdataTobyte;
	vector<unsigned char> tmp = intToByte(pd.anchor_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.anchor_chip_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.anchor_btooth_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.anchor_mac_adr);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = intToByte(pd.tag_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.tag_chip_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.tag_btooth_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = floatToByte(pd.distance);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	packetdata_length = packetdataTobyte.size();

	return packetdataTobyte;
}

