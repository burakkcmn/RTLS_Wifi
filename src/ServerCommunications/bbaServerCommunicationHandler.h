/*
 * bbaServerCommunicationHandler.h
 *
 *  Created on: Jan 17, 2022
 *      Author: aress
 */

#ifndef BBASERVERCOMMUNICATIONHANDLER_H_
#define BBASERVERCOMMUNICATIONHANDLER_H_

#include "../DataPackets/bbaPacketBase.h"
#include "bbaUdpClientManager.h"

class bbaServerCommunicationHandler {
private:
	bbaUdpClientManager udpManager;
public:
	bbaServerCommunicationHandler();
	virtual ~bbaServerCommunicationHandler();
	void begin();
	void debugLog(String message);
	string receive();
	bool registration(const int &anchorid, const string &anchorchipid,
			const string &anchorbtoothid, const string &anchormacadr,
			const string &anchorEspVersion, const string &anchorDwmVersion);
	bool sendLocation(const int &anchorid, const string &anchorchipid,
			const string &anchorbtoothid, const string &anchormacadr,
			const int &tag_id, const string &tag_chip_id,
			const string &tag_btooth_id, const float &distance);
	bool sendLog(const int &anchorid, const string &anchorchipid,
			const string &anchorbtoothid, const std::string &anchormacadr,
			const MESSAGECONTENT &Content, const std::string &message);
};

#endif /* BBASERVERCOMMUNICATIONHANDLER_H_ */
