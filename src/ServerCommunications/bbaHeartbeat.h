/*
 * bbaHeartbeat.h
 *
 *  Created on: Jan 29, 2022
 *      Author: aress
 */

#ifndef SERVERCOMMUNICATIONS_BBAHEARTBEAT_H_
#define SERVERCOMMUNICATIONS_BBAHEARTBEAT_H_

#include <WiFiUdp.h>
#include <string>
#include <vector>

#include "../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERIAL_HEARTBEAT_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#else
#include <Arduino.h>
#endif

using namespace std;

class bbaHeartbeat {

private:
	unsigned int heartbeatPort;
	WiFiUDP UDPclient;
	std::string serverIp = "";
	int serverPort = -1;
	vector<char> packetBuffer;
	bool hasPacket = false;
	std::string desiredMessage = "heartbeat!";

public:
	bbaHeartbeat();
	virtual ~bbaHeartbeat();

	void begin();
	std::string getPacket();
	unsigned char send(const std::string &ip, const uint16_t &port,
			const std::vector<unsigned char> &packet);
	void receive();
	void checkHeartbeat();

};

#endif /* SERVERCOMMUNICATIONS_BBAHEARTBEAT_H_ */
