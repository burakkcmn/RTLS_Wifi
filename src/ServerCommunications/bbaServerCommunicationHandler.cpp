/*
 * bbaServerCommunicationHandler.cpp
 *
 *  Created on: Jan 17, 2022
 *      Author: aress
 */

#include "bbaServerCommunicationHandler.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#include "bbaUdpClientManager.h"
#include "../wiredCommunication/serial/bbaSerial.h"
#include "../DataPackets/bbaPacketBase.h"
#include "../DataPackets/bbaRegistrationPacket.h"
#include "../DataPackets/bbaLocationPacket.h"
#include "../DataPackets/bbaDebugPacket.h"
#include "../utils/symbols/bbaGlobalSymbols.h"

bbaServerCommunicationHandler::bbaServerCommunicationHandler() {
	udpManager.begin(bbaGlobalSymbols::getServerUdpPortNumber());
}

bbaServerCommunicationHandler::~bbaServerCommunicationHandler() {
	udpManager.~bbaUdpClientManager();
}

void bbaServerCommunicationHandler::begin() {
	udpManager.begin(bbaGlobalSymbols::getESPUdpPortNumber());
}

string bbaServerCommunicationHandler::receive() {
	string message = "";
	udpManager.receive();
	if (udpManager.hasMassage()) {
		message = udpManager.getPacket();
	}
	return message;
}

bool bbaServerCommunicationHandler::registration(const int &anchorid,
		const string &anchorchipid, const string &anchorbtoothid,
		const string &anchormacadr, const string &anchorEspVersion,
		const string &anchorDwmVersion) {
	bbaRegistrationPacket packet(MESSAGESOURCES::s_ESP,
			MESSAGESOURCES::d_SERVER, MESSAGECONTENT::REGISTRATION);
	packet.createData(anchorid, anchorchipid, anchorbtoothid, anchormacadr,
			anchorEspVersion, anchorDwmVersion);

	string serverIp = bbaGlobalSymbols::getServerIpAddress();
	uint16_t serverPort = bbaGlobalSymbols::getServerUdpPortNumber();

	return udpManager.send(serverIp, serverPort, packet.getPacket());
}

bool bbaServerCommunicationHandler::sendLocation(const int &anchorid,
		const string &anchorchipid, const string &anchorbtoothid,
		const string &anchormacadr, const int &tag_id,
		const string &tag_chip_id, const string &tag_btooth_id,
		const float &distance) {
	bbaLocationPacket packet(MESSAGESOURCES::s_ESP, MESSAGESOURCES::d_SERVER,
			MESSAGECONTENT::LOCATION);
	packet.createData(tag_id, tag_chip_id, tag_btooth_id, anchorid,
			anchorchipid, anchorbtoothid, anchormacadr, distance);

	string serverIp = bbaGlobalSymbols::getServerIpAddress();
	uint16_t serverPort = bbaGlobalSymbols::getServerUdpPortNumber();

	return udpManager.send(serverIp, serverPort, packet.getPacket());
}

bool bbaServerCommunicationHandler::sendLog(const int &anchorid,
		const string &anchorchipid, const string &anchorbtoothid,
		const std::string &anchormacadr, const MESSAGECONTENT &Content,
		const std::string &message) {
	bbaDebugPacket packet(Content);
	packet.createData(anchorid, anchorchipid, anchorbtoothid, anchormacadr,
			message);

	string serverIp = bbaGlobalSymbols::getServerIpAddress();
	uint16_t serverPort = bbaGlobalSymbols::getServerUdpPortNumber();

	return udpManager.send(serverIp, serverPort, packet.getPacket());
}

