/*
 * bbaDebugPacket.cpp
 *
 *  Created on: Jan 28, 2022
 *      Author: aress
 */

#include "bbaDebugPacket.h"

bbaDebugPacket::bbaDebugPacket(const MESSAGECONTENT &Content) {
	source = MESSAGESOURCES::s_ESP;
	destination = MESSAGESOURCES::d_SERVER;
	content = Content;
}

bbaDebugPacket::~bbaDebugPacket() {
	// TODO Auto-generated destructor stub
}

void bbaDebugPacket::createData(const int &anchorid, const string &anchorchipid,
		const std::string &anchorbtoothid, const std::string &anchormacadr,
		const std::string &message) {
	packetData.anchor_btooth_id = anchorbtoothid;
	packetData.anchor_chip_id = anchorchipid;
	packetData.anchor_id = anchorid;
	packetData.anchor_mac_adr = anchormacadr;
	packetData.messageLength = message.length();
	packetData.message = message;
}

vector<unsigned char> bbaDebugPacket::intToByte(const int &number) {
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (number >> (i * 8));
	return std::move(arrayOfByte);
}

vector<unsigned char> bbaDebugPacket::stringToByte(const String &str) {
	vector<unsigned char> bytes(str.begin(), str.end());
	bytes.push_back('\0');
	return std::move(bytes);
}

vector<unsigned char> bbaDebugPacket::stringToByte(const std::string &str) {
	vector<unsigned char> bytes(str.begin(), str.end());
	bytes.push_back('\0');
	return std::move(bytes);
}

vector<unsigned char> bbaDebugPacket::getPacket() {
	vector<unsigned char> packetTobyte;

	vector<unsigned char> tmp = intToByte(source);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	tmp = intToByte(destination);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	tmp = intToByte(content);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	vector<unsigned char> packetDataByte = packetDataToByte(packetData);

	tmp = intToByte(packetdata_length);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	packetTobyte.insert(packetTobyte.end(), packetDataByte.begin(),
			packetDataByte.end());

	uint32_t crc = calculateCRC(packetTobyte);
	tmp = intToByte(crc);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	packetTobyte.insert(packetTobyte.begin(), 1, beginChar);
	packetTobyte.push_back(endChar);

#if ISDEFINED(SERIAL_LOG_PACKET_LOG)
	//Print packet...
	bbaSerial::dbg_printf(
			"DEBUG PACKET:\nSource:%d\nDestination:%d\nContent:%d\nTotal Length:%d\n",
			source, destination, content, packetdata_length);

	bbaSerial::dbg_printf(
			"Anchor Id:%d\nAnchor Chip Id:%s\nAnchor Bluetooth Id:%s\nAnchor Mac Adr:%s\n",
			packetData.anchor_id, packetData.anchor_chip_id.c_str(),
			packetData.anchor_btooth_id, packetData.anchor_mac_adr.c_str());

	bbaSerial::dbg_printf("message length:%d\nmessage:%s\n",
			packetData.messageLength, packetData.message.c_str());
	//Print vector...
	int len = packetTobyte.size();
	bbaSerial::dbg_printf("DEBUG PACKET: len->%d	packet->", len);
	for (int i = 0; i < len; ++i) {
		bbaSerial::dbg_printf("%d - ", packetTobyte[i]);
	}
	bbaSerial::dbg_printf("\n");
#endif

	return std::move(packetTobyte);
}

uint32_t bbaDebugPacket::calculateCRC(const vector<unsigned char> &data) {
	FastCRC32 CRC32;
	return CRC32.crc32(&data[0], data.size());
}

vector<unsigned char> bbaDebugPacket::packetDataToByte(const PACKETDATA &pd) {
	vector<unsigned char> packetdataTobyte;
	vector<unsigned char> tmp = intToByte(pd.anchor_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.anchor_chip_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.anchor_btooth_id);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.anchor_mac_adr);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = intToByte(pd.messageLength);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.message);
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	packetdata_length = packetdataTobyte.size();

	return std::move(packetdataTobyte);
}

