/*
 * bbaMessage.h
 *
 *  Created on: Feb 20, 2022
 *      Author: aress
 */

#ifndef WIREDCOMMUNICATION_BBAMESSAGEBUFFERBUFFER_H_
#define WIREDCOMMUNICATION_BBAMESSAGEBUFFERBUFFER_H_

#include "../../utils/symbols/bbaGlobalSymbols.h"

#if ISDEFINED(SERIAL_MESSAGEBUFFER_LOG)
#include "bbaSerial.h"
#else
#include <string>
#endif

using namespace std;

class bbaMessageBuffer {

private:
	const char messageBegin = '#';
	const char messageEnd = '$';
	string buffer;

	string parsMessage();

public:
	bbaMessageBuffer();
	virtual ~bbaMessageBuffer();
	void add(const std::string &msg);
	string getMessage();
	int getLength();
	string getBuffer();
	bool compareWithBeginChar(const char &chr, int &beginIndex);
	bool compareWithEndChar(const char &chr, int &endIndex);
	bool isBeginIndexAlreadyFound(const int &beginIndex, const int &index);
	bool isBeginIndexFound(const int &beginIndex, const int &index);
	void deleteRangeFromBuffer(const int &startIndex, const int &endIndex);
	string createMessage(const int &startIndex, const int &endIndex);

};
#endif /* WIREDCOMMUNICATION_BBAMESSAGEBUFFERBUFFER_H_ */
