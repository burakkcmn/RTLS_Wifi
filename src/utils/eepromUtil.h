/*
 * eepromUtil.h
 *
 *  Created on: Jan 1, 2023
 *      Author: aress
 */

#ifndef UTILS_EEPROMUTIL_H_
#define UTILS_EEPROMUTIL_H_

#include <iostream>
#include <EEPROM.h>
#include "../wiredCommunication/serial/bbaSerial.h"

using namespace std;

class eepromUtil final {
public:
	static void begin();
	static bool end();

	enum WIFIADRESS {
		AP_SSID = 0, // 35 byte
		AP_PASSWORD = 35, // 35 byte
		AP_CHANNEL = 70, // 4 byte
		AP_HIDDEN = 74, // bit 0
		AP_MAXCONN = 75, // 4 byte
		STA_SSID = 79, // 35 byte
		STA_PASSWORD = 114, // 35 byte
		STA_CHANNEL = 149, // 4 byte
		STA_HIDDEN = 74, // bit 1
		STA_MAXCONN = 153 // 4 byte
	};

	enum BITADDRESS {
		AP_HIDDEN_BIT = 0, //bit0
		STA_HIDDEN_BIT = 1
	};

	enum ANCHORINFO {
		ID = 157,	// 4 byte
		VERSION_MAJOR = 160, // 4 byte
		VERSION_MINOR = 164 // 8 byte
	};

	enum SERVERINFO {
		IP = 172,	// 15 byte
		PORT = 187 // 4 byte
	};

	static bool write(const int &address, const string &data,
			const uint8_t &length);
	static string readString(const int &address, const uint8_t &length);

	static bool write(const int &address, const int &data);
	static int readInt(const int &address);

	static bool write(const int &address, const float &data);
	static float readFloat(const int &address);

	static bool write(const int &address, const uint8_t &bit, const bool &data);
	static bool readbool(const int &address, const uint8_t &bit);

private:
	static bool write(const int &address, const uint8_t *data,
			const uint8_t &length);
	static uint8_t* read(const int &address, const uint8_t &length);
};

#endif /* UTILS_EEPROMUTIL_H_ */
