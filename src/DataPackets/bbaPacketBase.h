/*
 * bbaPacketBase.h
 *
 *  Created on: Jan 26, 2022
 *      Author: aress
 */

#ifndef DATAPACKETS_BBAPACKETBASE_H_
#define DATAPACKETS_BBAPACKETBASE_H_

enum MESSAGESOURCES {
	d_SERVER = 0, //
	s_DWM, //
	s_ESP
};

enum MESSAGECONTENT {
	LOCATION = 0, //
	INFORMATION, //
	REGISTRATION, //
	FIRMWARE, //
	DEBUG, //
	ERROR, //
	WARNING
};

#endif /* DATAPACKETS_BBAPACKETBASE_H_ */
