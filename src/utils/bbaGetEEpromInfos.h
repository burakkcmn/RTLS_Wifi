/*
 * bbaGetEEpromInfos.h
 *
 *  Created on: Mar 27, 2023
 *      Author: aress
 */

#ifndef UTILS_BBAGETEEPROMINFOS_H_
#define UTILS_BBAGETEEPROMINFOS_H_

#include <string>

class bbaGetEEpromInfos final {
private:

public:
	bbaGetEEpromInfos();
	virtual ~bbaGetEEpromInfos();

	static std::string getVersionFromEEprom();
	static int getAnchorIdFromEEprom();
	static std::string getServerIpFromEEprom();
	static int getServerPortFromEEprom();

};

#endif /* UTILS_BBAGETEEPROMINFOS_H_ */
