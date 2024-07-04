/*
 * bbaNrf52xxBootloader.h
 *
 *  Created on: 8 A?u 2023
 *      Author: burak
 */

#ifndef WIREDCOMMUNICATION_BOOTLOADER_BBANRF52XXBOOTLOADER_H_
#define WIREDCOMMUNICATION_BOOTLOADER_BBANRF52XXBOOTLOADER_H_

#include <Arduino.h>
#include <stdint.h>
#include <Adafruit_DAP.h>
#include <dap.h>
#include <dap_config.h>
#include "../../utils/symbols/bbaGlobalSymbols.h"

class bbaNrf52xxBootloader {
private:
	typedef struct line_desc {
		uint8_t LL;                      //->Record Length
		uint16_t AAAA;                    //->Address
		uint8_t TT;                      //->Record Type
		enum RecordType {
			data = 0,
			end_of_file = 1,
			extended_segment_address = 3,
			extended_linear_address = 4,
			start_linear_address = 5,       //(MDK-ARM only)
		};
		uint8_t DD[16];                  //->Data
		uint8_t CC;                      //->Checksum
	};                      //, *pline_desc;

	bool checksum(String *Line);
	bool frmString(String &line);
	static void error(const char *text);
	bool stringToBuf(String *File, uint32_t &fileSize, uint16_t *bufsize);
	bool GetFile(String *File, uint32_t *fileSize, const String &path_);

//	std::vector<uint8_t> buff;
	uint8_t *buff;
	Adafruit_DAP_nRF5x dap;
	line_desc linedesc;

public:
	bbaNrf52xxBootloader();
	virtual ~bbaNrf52xxBootloader();
	bool checkNewFirmware(String fileUrl);
};

#endif /* WIREDCOMMUNICATION_BOOTLOADER_BBANRF52XXBOOTLOADER_H_ */
