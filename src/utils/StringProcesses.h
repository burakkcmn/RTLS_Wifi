/*
 * wstringProcesses.h
 *
 *  Created on: May 15, 2022
 *      Author: aress
 */

#ifndef UTILS_STRINGPROCESSES_H_
#define UTILS_STRINGPROCESSES_H_

#include "../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERIAL_STRING_PROCESS_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#else
#include <string>
#endif

#include <vector>
#include <arduino.h>

using namespace std;

class StringProcesses {
public:
	StringProcesses();
	virtual ~StringProcesses();

	static vector<string> split(const std::string &str,
			const std::string &delimiter);
	static void removeBeginAndEndChars(string &str);
	static float convertFloat(const string &str);
	static bool isNumber(const std::string &s);
	static int convertInt(const string &str);
	static void add(string &str1, const string &str2);
	static string create(const char *format, ...);
	static bool checkHex(const string &str);
	static int arrayLength(const string array[]);
	static int indexOf(const string &source, const string &key);
	static bool contains(const string &source, const string &key);
	static void toUpperCase(string &data);
	static String stdStringToString(const string &str);
	static string stringToStdString(const String &str);

};

#endif /* UTILS_STRINGPROCESSES_H_ */
