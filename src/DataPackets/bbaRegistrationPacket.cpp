/*
 * bbaRegistrationPacket.cpp
 *
 *  Created on: Jan 26, 2022
 *      Author: aress
 */

#include "bbaRegistrationPacket.h"

bbaRegistrationPacket::bbaRegistrationPacket(const MESSAGESOURCES &msSource,
		const MESSAGESOURCES &msDestination, const MESSAGECONTENT &mcContent) {
	source = msSource;
	destination = msDestination;
	content = mcContent;
}

bbaRegistrationPacket::~bbaRegistrationPacket() {
	// TODO Auto-generated destructor stub
}

void bbaRegistrationPacket::createData(const int &anchorid,
		const string &anchorchipid, const string &anchorbtoothid,
		const string &anchormacadr, const string &anchorEspVersion,
		const string &anchorDwmVersion) {
	packetData.anchor_btooth_id = anchorbtoothid;
	packetData.anchor_chip_id = anchorchipid;
	packetData.anchor_id = anchorid;
	packetData.anchor_mac_adr = anchormacadr;
	packetData.anchorEspVersion = anchorEspVersion;
	packetData.anchorDwmVersion = anchorDwmVersion;
}

vector<unsigned char> bbaRegistrationPacket::intToByte(const int &number) {
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (number >> (i * 8)) & 0xFF;
	return arrayOfByte;
}

vector<unsigned char> bbaRegistrationPacket::stringToByte(const string &str) {
	vector<unsigned char> bytes(str.begin(), str.end());
	bytes.push_back('\0');
	return bytes;
}

vector<unsigned char> bbaRegistrationPacket::getPacket() {
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
//	bbaSerial::dbg_printf("%u\n", crc);
	tmp = intToByte(crc);
	packetTobyte.insert(packetTobyte.end(), tmp.begin(), tmp.end());

	packetTobyte.insert(packetTobyte.begin(), 1, beginChar);
	packetTobyte.push_back(endChar);

#if ISDEFINED(SERIAL_REGISTRATION_PACKET_LOG)
	//Print packet...
	bbaSerial::dbg_printf(
			"\n\REGISTER PACKET:\nSource:%d\nDestination:%d\nContent:%d\nTotal Length:%d\n",
			source, destination, content, packetdata_length);

	bbaSerial::dbg_printf(
			"Anchor Id:%d\nAnchor Chip Id:%s\nAnchor Bluetooth Id:%s\nAnchor Mac Adr:%s\n",
			packetData.anchor_id, packetData.anchor_chip_id.c_str(),
			packetData.anchor_btooth_id, packetData.anchor_mac_adr.c_str());

	bbaSerial::dbg_printf(
			"ESP version: %s, DWM version: %s, Tag version: %s\n\n",
			packetData.anchorEspVersion, packetData.anchorDwmVersion,
			packetData.tagDwmVersion);
	//Print vector...
	int len = packetTobyte.size();
	bbaSerial::dbg_printf("REGISTER PACKET: len->%d	packet->", len);
	for (int i = 0; i < len; ++i) {
		bbaSerial::dbg_printf("%d - ", packetTobyte[i]);
	}
	bbaSerial::dbg_printf("\n\n");
#endif

	//Print vector...
//	int len = packetTobyte.size();
//	bbaSerial::dbg_printf("\nREGISTER: len->%d	packet->", len);
//	for (int i = 0; i < len; ++i) {
//		bbaSerial::dbg_printf("%x - ", packetTobyte[i]);
//	}
//	bbaSerial::dbg_printf("\n");

	return packetTobyte;
}

uint32_t bbaRegistrationPacket::calculateCRC(
		const vector<unsigned char> &data) {
	FastCRC32 CRC32;
	return CRC32.crc32(&data[0], data.size());
}

vector<unsigned char> bbaRegistrationPacket::packetDataToByte(
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

	tmp = stringToByte(pd.anchorEspVersion);
	vector<unsigned char> len = intToByte(tmp.size());
	packetdataTobyte.insert(packetdataTobyte.end(), len.begin(), len.end());
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.anchorDwmVersion);
	len = intToByte(tmp.size());
	packetdataTobyte.insert(packetdataTobyte.end(), len.begin(), len.end());
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	tmp = stringToByte(pd.tagDwmVersion);
	len = intToByte(tmp.size());
	packetdataTobyte.insert(packetdataTobyte.end(), len.begin(), len.end());
	packetdataTobyte.insert(packetdataTobyte.end(), tmp.begin(), tmp.end());

	packetdata_length = packetdataTobyte.size();

	return packetdataTobyte;
}

