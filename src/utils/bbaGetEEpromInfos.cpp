/*
 * bbaGetEEpromInfos.cpp
 *
 *  Created on: Mar 27, 2023
 *      Author: aress
 */

#include "bbaGetEEpromInfos.h"

#include "eepromUtil.h"

bbaGetEEpromInfos::bbaGetEEpromInfos() {
	// TODO Auto-generated constructor stub

}

bbaGetEEpromInfos::~bbaGetEEpromInfos() {
	// TODO Auto-generated destructor stub
}

std::string bbaGetEEpromInfos::getVersionFromEEprom() {
	std::string major = eepromUtil::readString(eepromUtil::ANCHORINFO::VERSION_MAJOR, 4);
	std::string minor = eepromUtil::readString(eepromUtil::ANCHORINFO::VERSION_MINOR, 8);
	return major + minor;
}

int bbaGetEEpromInfos::getAnchorIdFromEEprom() {
	return eepromUtil::readInt(eepromUtil::ANCHORINFO::ID);
}

std::string bbaGetEEpromInfos::getServerIpFromEEprom() {
	return eepromUtil::readString(eepromUtil::SERVERINFO::IP, 15);
}

int bbaGetEEpromInfos::getServerPortFromEEprom() {
	return eepromUtil::readInt(eepromUtil::SERVERINFO::PORT);
}
