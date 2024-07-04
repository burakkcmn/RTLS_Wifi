/*
 * bbaUdpClientManager.h
 *
 *  Created on: Jan 17, 2022
 *      Author: aress
 */

#ifndef BBAUDPCLIENTMANAGER_H_
#define BBAUDPCLIENTMANAGER_H_

#include <Arduino.h>
#include <vector>
#include <iostream>
#include <WiFiUdp.h>

using namespace std;

class bbaUdpClientManager {
private:
	WiFiUDP UDPclient;
	String uart_receive_string;
	char packetBuffer[1024];       //buffer to hold incoming packet
	bool hasPacket = false;

public:
	bbaUdpClientManager();
	virtual ~bbaUdpClientManager();
	void begin(const int &port);
	string getPacket();
	bool send(const string&, const uint16_t&, const vector<unsigned char>&);
	void receive();
	bool hasMassage();
};

#endif /* BBAUDPCLIENTMANAGER_H_ */
