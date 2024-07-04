/*
 * bbaESPInformations.h
 *
 *  Created on: Feb 22, 2023
 *      Author: aress
 */

#ifndef UTILS_BBAESPINFORMATIONS_H_
#define UTILS_BBAESPINFORMATIONS_H_

#include "symbols/bbaGlobalSymbols.h"

class bbaESPInformations final {
public:
	bbaESPInformations();
	virtual ~bbaESPInformations();
	static std::string getResetInformation();
	static std::string getResetReason();
	static std::string getResetInfo();
	static std::string getInformation();
};

#endif /* UTILS_BBAESPINFORMATIONS_H_ */
