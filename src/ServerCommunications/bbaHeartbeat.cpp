/*
 * bbaHeartbeat.cpp
 *
 *  Created on: Jan 29, 2022
 *      Author: aress
 */

#include "bbaHeartbeat.h"

#if ISDEFINED(SERVER_HEARTBEAT_LOG)
#include "../utils/bbaServerLog.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#endif

bbaHeartbeat::bbaHeartbeat() {
	packetBuffer.resize(128);
}

bbaHeartbeat::~bbaHeartbeat() {

}

void bbaHeartbeat::begin() {
	UDPclient.begin(bbaGlobalSymbols::getHeartbeatNumber());
#if ISDEFINED(SERIAL_MAIN_LOG)
	bbaSerial::dbg_printf("[HEARTBEAT] Listening on UDP port %u\n\n",
			bbaGlobalSymbols::getHeartbeatNumber());
#endif
#if ISDEFINED(SERVER_HEARTBEAT_LOG)
	if (bbaLogSymbols::getHeartbeatLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("[HEARTBEAT] Listening on UDP port %u",
				bbaGlobalSymbols::getHeartbeatNumber());
	}
#endif
}

std::string bbaHeartbeat::getPacket() {
	std::string tpacket;
	if (hasPacket) {
		tpacket.assign(packetBuffer.begin(), packetBuffer.end());
		packetBuffer.clear();
		hasPacket = false;
	}
	return tpacket;
}

unsigned char bbaHeartbeat::send(const std::string &ip, const uint16_t &port,
		const std::vector<unsigned char> &packet) {
	std::string s(reinterpret_cast<const char*>(&packet[0]), packet.size());
#if ISDEFINED(SERIAL_MAIN_LOG)
	bbaSerial::dbg_printf("[HEARTBEAT] ip : %s - port : %d - packet : %s",
			ip.c_str(), port, s.c_str());
#endif
#if ISDEFINED(SERVER_HEARTBEAT_LOG)
	if (bbaLogSymbols::getHeartbeatLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log("[HEARTBEAT] ip : %s - port : %d - packet : %s",
				ip.c_str(), port, s.c_str());
	}
#endif
	UDPclient.beginPacket(ip.c_str(), port);
	UDPclient.write(&packet[0], packet.size());
	return UDPclient.endPacket();
}

void bbaHeartbeat::receive() {
	int packetSize = UDPclient.parsePacket();
	if (packetSize > 0) {
		int len = UDPclient.read(&packetBuffer[0], packetBuffer.size());
		if (len > 0) {
			packetBuffer[len] = '\0';
			serverIp = UDPclient.remoteIP().toString().c_str();
			serverPort = UDPclient.remotePort();
#if ISDEFINED(SERIAL_MAIN_LOG)
			bbaSerial::dbg_printf(
					"[HEARTBEAT] Receive packet : %s from %s:%d\n",
					packetBuffer.data(), serverIp.c_str(), serverPort);
#endif
#if ISDEFINED(SERVER_HEARTBEAT_LOG)
			if (bbaLogSymbols::getHeartbeatLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::dbg_log(
						"[HEARTBEAT] Receive packet : %s from %s:%d\n",
						packetBuffer.data(), serverIp.c_str(), serverPort);
			}
#endif
			hasPacket = true;
		}
	}
}

void bbaHeartbeat::checkHeartbeat() {
	receive();
	if (hasPacket) {
		std::string message = getPacket();
		if (message.compare(desiredMessage) == 0) {
#if ISDEFINED(SERIAL_MAIN_LOG)
			bbaSerial::dbg_printf(
					"[HEARTBEAT] Message : %s check successfully.\n",
					message.c_str());
#endif
#if ISDEFINED(SERVER_HEARTBEAT_LOG)
			if (bbaLogSymbols::getHeartbeatLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::dbg_log(
						"[HEARTBEAT] Message : %s check successfully.\n",
						message.c_str());
			}
#endif
			std::vector<unsigned char> v(message.begin(), message.end());
			send(serverIp, serverPort, v);
		}
	}
}
