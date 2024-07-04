/*
 * convertUtil.h
 *
 *  Created on: Jan 1, 2023
 *      Author: aress
 */

#ifndef UTILS_CONVERTUTIL_H_
#define UTILS_CONVERTUTIL_H_

#include <iostream>

using namespace std;

class convertUtil final {
public:
	static uint8_t* stringToByteArray(const string &str);
	static string byteArrayToString(const uint8_t *array,
			const uint8_t &length);

	static uint8_t* intToByteArray(const int &number);
	static int byteArrayToInt(const uint8_t *array);

	static int stringToInt(const string &number);

	static bool stringToBool(const string &bdata);

	static std::string macToString(const unsigned char *mac);

	static uint8_t hexTouint8t(const char &hexdigit);
	static char uint8tToHex(const uint8_t &value);
};

#endif /* UTILS_CONVERTUTIL_H_ */
