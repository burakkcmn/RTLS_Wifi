/*
 * bbaServerCommandHandler.h
 *
 *  Created on: Mar 26, 2023
 *      Author: aress
 */

#ifndef SERVERCOMMUNICATIONS_BBASERVERCOMMANDHANDLER_H_
#define SERVERCOMMUNICATIONS_BBASERVERCOMMANDHANDLER_H_

#include "bbaServerCommunicationHandler.h"

class bbaServerCommandHandler final {
private:
	bbaServerCommunicationHandler serverCommunication;

	vector<string> splitTokens(const string &message);
	bool isRestartTag(const string &tag);
	bool isAnchorIdChangeTag(const string &tag);
	bool isTagIdChangeTag(const string &tag);
	bool isAnchorWifiLedBlinkTag(const string &tag);
	bool isAnchorWifiLedOnTag(const string &tag);
	bool isStopRunningTag(const string &tag);
	bool isRegisterRequestTag(const string &tag);

public:
	bbaServerCommandHandler();
	virtual ~bbaServerCommandHandler();

	void process();
};

#endif /* SERVERCOMMUNICATIONS_BBASERVERCOMMANDHANDLER_H_ */
