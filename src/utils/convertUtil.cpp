/*
 * convertUtil.cpp
 *
 *  Created on: Jan 1, 2023
 *      Author: aress
 */

#include "convertUtil.h"
#include <vector>
#include <cstring>

uint8_t* convertUtil::stringToByteArray(const string &str) {
	std::vector<uint8_t> buffer(str.length() + 1);
	memcpy(buffer.data(), str.c_str(), str.length() + 1);
	return buffer.data();
}

string convertUtil::byteArrayToString(const uint8_t *array,
		const uint8_t &length) {
	string s(reinterpret_cast<const char*>(array), length);
	return s;
}

uint8_t* convertUtil::intToByteArray(const int &number) {
	uint8_t *bytes = new uint8_t[4];

	bytes[0] = (number >> 24) & 0xFF;
	bytes[1] = (number >> 16) & 0xFF;
	bytes[2] = (number >> 8) & 0xFF;
	bytes[3] = (number >> 0) & 0xFF;

	return bytes;
}

int convertUtil::byteArrayToInt(const uint8_t *array) {
	int value;
	value = (value << 8) + array[0];
	value = (value << 8) + array[1];
	value = (value << 8) + array[2];
	value = (value << 8) + array[3];
	return value;
}

int convertUtil::stringToInt(const string &number) {
	return std::stoi(number);
}

bool convertUtil::stringToBool(const string &bdata) {
	if (bdata == "true" || bdata == "1") {
		return true;
	}
	return false;
}

std::string convertUtil::macToString(const unsigned char *mac) {
	char buf[20];
	snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1],
			mac[2], mac[3], mac[4], mac[5]);
	return buf;
}

uint8_t convertUtil::hexTouint8t(const char &hexdigit) {
	const char hexChars[] = "0123456789ABCDEFabcdef";
	char *ptr = strchr(hexChars, hexdigit);
	if (ptr) {
		return (uint8_t) (ptr - hexChars) % 16;
	} else {
		//Invalid hexa digit
		return 17;
	}
}

char convertUtil::uint8tToHex(const uint8_t &value) {
	const char hexChars[] = "0123456789ABCDEF";
	if (value < 16) {
		return hexChars[value];
	} else {
		//Invalid uint8_t value
		return 'X';
	}
}
