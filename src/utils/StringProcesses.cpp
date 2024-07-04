/*
 * wstringProcesses.cpp
 *
 *  Created on: May 15, 2022
 *      Author: aress
 */

#include "StringProcesses.h"
#include <algorithm>
#include <stdexcept>

StringProcesses::StringProcesses() {
// TODO Auto-generated constructor stub

}

StringProcesses::~StringProcesses() {
// TODO Auto-generated destructor stub
}

std::vector<std::string> StringProcesses::split(const std::string &str,
		const std::string &delimiter) {
	std::vector<std::string> res;
	size_t pos_start = 0;
	size_t pos_end = 0;
	size_t delim_len = delimiter.length();
	std::string token;

	while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
		token = str.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}
	res.push_back(str.substr(pos_start));
	return std::move(res);
}

void StringProcesses::removeBeginAndEndChars(string &str) {
	str = str.substr(1, str.length() - 2);
}

float StringProcesses::convertFloat(const string &str) {
	return std::stof(str);
}

bool StringProcesses::isNumber(const std::string &s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

int StringProcesses::convertInt(const string &str) {
	if (!isNumber(str)) {
		return -1;
	}
	return std::stoi(str);
}

void StringProcesses::add(string &str1, const string &str2) {
	str1.append(str2);
}

string StringProcesses::create(const char *format, ...) {
	constexpr size_t MaxBufferSize = 1400;
	char buffer[MaxBufferSize];

	va_list varArgs;
	va_start(varArgs, format);
	int result = vsnprintf(buffer, MaxBufferSize, format, varArgs);
	va_end(varArgs);

	if (result < 0 || static_cast<size_t>(result) >= MaxBufferSize) {
		throw std::runtime_error("Error while formatting string.");
	}

	return buffer;
}

bool StringProcesses::checkHex(const string &str) {
	int n = str.length();
	for (int i = 0; i < n; i++) {
		char ch = str[i];
		if ((ch < '0' || ch > '9') && (ch < 'A' || ch > 'F')
				&& (ch < 'a' || ch > 'f')) {
			return false;
		}
	}
	return true;
}

int StringProcesses::arrayLength(const string array[]) {
	int returnValue = sizeof(array) / sizeof(array[0]);
	return returnValue;
}

int StringProcesses::indexOf(const string &source, const string &key) {
	int returnValue = source.find(key);
	if (returnValue == string::npos) {
		return -1;
	} else {
		return returnValue;
	}
}

bool StringProcesses::contains(const string &source, const string &key) {
	bool returnValue = indexOf(source, key) != string::npos;
	if (returnValue) {
		return true;
	} else {
		return false;
	}
}

void StringProcesses::toUpperCase(string &data) {
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
}

String StringProcesses::stdStringToString(const string &str) {
	String Str(str.c_str());
	return Str;
}

string StringProcesses::stringToStdString(const String &str) {
	std::string stdStr(str.c_str(), str.length());
	return stdStr;
}
