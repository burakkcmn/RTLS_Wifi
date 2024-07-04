/*
 * bbaUdpClientManager.cpp
 *
 *  Created on: Jan 17, 2022
 *      Author: aress
 */

#include "bbaUdpClientManager.h"

#include "../wiredCommunication/serial/bbaSerial.h"
#include "../utils/bbaServerLog.h"
#include "../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERIAL_UDP_CLIENT_MANAGER_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#endif

bbaUdpClientManager::bbaUdpClientManager() {
	UDPclient.begin(bbaGlobalSymbols::getESPUdpPortNumber());
#if ISDEFINED(SERIAL_UDP_CLIENT_MANAGER_LOG)
	bbaSerial::dbg_printf("Listening on UDP port %d\n",
			bbaGlobalSymbols::getESPUdpPortNumber());
#endif
}

bbaUdpClientManager::~bbaUdpClientManager() {
	UDPclient.stop();
}

void bbaUdpClientManager::begin(const int &port) {
	UDPclient.begin(port);
#if ISDEFINED(SERIAL_UDP_CLIENT_MANAGER_LOG)
	bbaSerial::dbg_printf("Listening on UDP port %d begin\n", port);
#endif
}

bool bbaUdpClientManager::hasMassage() {
	return hasPacket;
}

string bbaUdpClientManager::getPacket() {
	string tpacket;
	if (hasPacket) {
		tpacket = packetBuffer;
		packetBuffer[0] = 0;
		hasPacket = false;
	}
	return tpacket;
}

bool bbaUdpClientManager::send(const string &ip, const uint16_t &port,
		const vector<unsigned char> &packet) {
	bool success = UDPclient.beginPacket(ip.c_str(), port);
	if (success) {
		UDPclient.write(packet.data(), packet.size());
		unsigned long timeoutStart = millis();
		unsigned long timeoutDuration = 5000;
		delay(0);
		success = false;
		while (!success && ((millis() - timeoutStart) < timeoutDuration)) {
			yield();
			success = UDPclient.endPacket();
		}
		delay(0);
	} else {
#if ISDEFINED(SERIAL_UDP_CLIENT_MANAGER_LOG)
		bbaSerial::dbg_printf(
				"[UDP Client Manager] - UDPclient begin connection result: %s\n\n",
				(success ? "true" : "false"));
#endif
	}
	return success;
}

void bbaUdpClientManager::receive() {
	int packetSize = UDPclient.parsePacket();
	if (packetSize > 0) {
		int len = UDPclient.read(packetBuffer, 2048);
		if (len > 0) {
			packetBuffer[len] = '\0';
#if ISDEFINED(SERIAL_UDP_CLIENT_MANAGER_LOG)
			bbaSerial::dbg_printf("[UDP Client Manager] - UDP Receive: %s.\n\n",
					packetBuffer);
#endif
			hasPacket = true;
		}
	}
}
