/*
 * bbaSerialHandler.h
 *
 *  Created on: May 8, 2022
 *      Author: aress
 */

#ifndef WIREDCOMMUNICATION_BBASERIALHANDLER_H_
#define WIREDCOMMUNICATION_BBASERIALHANDLER_H_

#include "../../ServerCommunications/bbaServerCommunicationHandler.h"
#include "bbaMessageBuffer.h"
#include "../../utils/bbaTimer.h"

class bbaSerialHandler {
public:
	bbaSerialHandler();
	virtual ~bbaSerialHandler();

	void process();

	bbaTimer myTimer;

private:
	bbaMessageBuffer MessageBuffer;
	bbaServerCommunicationHandler serverCommunication;

	void processRead();
	void processWrite();

	bool isMessageEmpty(const string *message);
	vector<string> splitTokens(const string *message);

	bool isLocationTag(const string *tag);
	bool isTokenSizeValid(const vector<string> *token, const int *size);
	string getTagChipId(const string *token);
	float getDistance(const string *token);
	bool sendLocationInfo(const int *tag_id, const string *tag_chip_id,
			const string *tag_btooth_id, const float *distance);

	bool isErrorTag(const string *tag);
	bool sendErrorLog(const string *message);

	bool isInfoTag(const string *tag);
	bool sendInfoLog(const string *message);

	bool isRegisterTag(const string *tag);
	bool getAnchorChipId(const string *token);
	bool getAnchorBluetoothId(const string *token);
	bool getDWMVersion(const string *token);

	bool sendRegisterInfo();

	bool isRestartTag(const string *tag);
};

#endif /* WIREDCOMMUNICATION_BBASERIALHANDLER_H_ */
