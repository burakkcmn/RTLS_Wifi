/*
 * bbaNrf52xxBootloader.cpp
 *
 *  Created on: 8 A?u 2023
 *      Author: burak
 */

#include "bbaNrf52xxBootloader.h"

#include "../../utils/convertUtil.h"
#include <ESP8266HTTPClient.h>
#include "../../utils/StringProcesses.h"

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
#include "../serial/bbaSerial.h"
#endif

#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
#include "../../utils/bbaServerLog.h"
#include "../../wirelessCommunication/bbaWifiManager.h"
#endif

bbaNrf52xxBootloader::bbaNrf52xxBootloader() {
	buff = new uint8_t[bbaGlobalSymbols::getDwmFirmwareBufferSize()];
}

bbaNrf52xxBootloader::~bbaNrf52xxBootloader() {
	delete[] buff;
}

bool bbaNrf52xxBootloader::checksum(String *Line) {
	uint8_t CC_sum = 0;
	for (int j = 0; j < (strlen(Line->c_str()) - 4); j += 2) {
		CC_sum += (convertUtil::hexTouint8t(Line->charAt(j)) * 16)
				+ convertUtil::hexTouint8t(Line->charAt(j + 1));
	}
	CC_sum = 1 + ((uint8_t) 255 - CC_sum);
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	if (linedesc.CC != CC_sum) {
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] Check: %d  -  Sum: %d\n",
				linedesc.CC, CC_sum);
	}
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		if (linedesc.CC != CC_sum) {
			bbaServerLog::error_log(
					"[DWM UPDATE MANAGER] Check: %d  -  Sum: %d", linedesc.CC,
					CC_sum);
		}
	}
#endif
	return linedesc.CC == CC_sum;
}

bool bbaNrf52xxBootloader::frmString(String &line) {
	linedesc.LL = convertUtil::hexTouint8t(line[0]) * 16
			+ convertUtil::hexTouint8t(line[1]);
	linedesc.CC = convertUtil::hexTouint8t(line[8 + linedesc.LL * 2]) * 16
			+ convertUtil::hexTouint8t(line[9 + linedesc.LL * 2]);
	/*if(!checksum(&line)){
	 #ifdef nrf52xx_dbg_err
	 dbg_printf("Checksum failed.\n");
	 #endif
	 return false;
	 }*/
	linedesc.AAAA = convertUtil::hexTouint8t(line[2]) * 4096
			+ convertUtil::hexTouint8t(line[3]) * 256
			+ convertUtil::hexTouint8t(line[4]) * 16
			+ convertUtil::hexTouint8t(line[5]);
	linedesc.TT = convertUtil::hexTouint8t(line[6]) * 16
			+ convertUtil::hexTouint8t(line[7]);
	for (int i = 0; i < linedesc.LL; i++) {
		memset(&linedesc.DD[i],
				((convertUtil::hexTouint8t(line[2 * i + 8])) << 4)
						+ convertUtil::hexTouint8t(line[2 * i + 9]),
				sizeof(uint8_t));
	}
	ESP.wdtFeed();
	return true;
}

void bbaNrf52xxBootloader::error(const char *text) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("[DWM UPDATE MANAGER] Error: %s\n", text);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::error_log("[DWM UPDATE MANAGER] Error: %s", text);
	}
#endif
}

bool bbaNrf52xxBootloader::stringToBuf(String *File, uint32_t &fileSize,
		uint16_t *bufsize) {
	dap.program_start();

	uint32_t firstAddr = convertUtil::hexTouint8t(File->charAt(3)) * 4096
			+ convertUtil::hexTouint8t(File->charAt(4)) * 256
			+ convertUtil::hexTouint8t(File->charAt(5)) * 16
			+ convertUtil::hexTouint8t(File->charAt(6));
	int lineLength = 0;
	line_desc ld;

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("\n");
	char buf_[255] = "";
	char DDint_[255] = "";
	char DDchar_[255] = "";
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		char buf_[255] = "";
		char DDint_[255] = "";
		char DDchar_[255] = "";
	}
#endif

	int16_t lineCount;
	for (lineCount = 0; lineCount < fileSize - 2;) {
		while (File->charAt(lineCount) != ':') {
			lineCount++;
		}
		int dataLength = convertUtil::hexTouint8t(File->charAt(lineCount + 1))
				* 16 + convertUtil::hexTouint8t(File->charAt(lineCount + 2));
		lineLength = 1 + 2 + 4 + 2 + (dataLength) * 2 + 2;
		String line = File->substring(lineCount + 1,
				lineCount + 1 + lineLength);

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] Line: %s\n",
				line.substring(0, lineLength - 1).c_str());
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[DWM UPDATE MANAGER] Line: %s",
					line.substring(0, lineLength - 1).c_str());
		}
#endif

		if (!frmString(line)) {
			return false;
		}

		*bufsize += ld.LL;

		for (int16_t charCountInLine = 0; charCountInLine < ld.LL;
				charCountInLine++) {

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
			sprintf(DDchar_ + strlen(DDchar_), "%c%c ",
					convertUtil::uint8tToHex(ld.DD[charCountInLine] >> 4),
					convertUtil::uint8tToHex(ld.DD[charCountInLine] & 0x0F));
			sprintf(DDint_ + strlen(DDint_), "%d ", ld.DD[charCountInLine]);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
			if (bbaLogSymbols::getNrf52xxBootloaderLog()
					&& bbaWifiManager::isConnected()) {
				sprintf(DDchar_ + strlen(DDchar_), "%c%c ",
						convertUtil::uint8tToHex(ld.DD[charCountInLine] >> 4),
						convertUtil::uint8tToHex(
								ld.DD[charCountInLine] & 0x0F));
				sprintf(DDint_ + strlen(DDint_), "%d ", ld.DD[charCountInLine]);
			}
#endif

			memcpy((void*) buff[ld.AAAA - firstAddr + charCountInLine],
					&ld.DD[charCountInLine], sizeof(uint8_t));

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
			sprintf(buf_ + strlen(buf_), "%d ",
					buff[ld.AAAA + charCountInLine]);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
			if (bbaLogSymbols::getNrf52xxBootloaderLog()
					&& bbaWifiManager::isConnected()) {
				sprintf(buf_ + strlen(buf_), "%d ",
						buff[ld.AAAA + charCountInLine]);
			}
#endif

		}

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf(
				"[DWM UPDATE MANAGER] data char: %s\tint: %s\tbuf int : %s    -> infos:  data length : %d  --  line size : %d  --  Address : %08x\n",
				DDchar_, DDint_, buf_, dataLength, lineLength, ld.AAAA);
		memset(buf_, 0, 255);
		memset(DDint_, 0, 255);
		memset(DDchar_, 0, 255);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log(
					"[DWM UPDATE MANAGER] data char: %s\tint: %s\tbuf int : %s    -> infos:  data length : %d  --  line size : %d  --  Address : %08x",
					DDchar_, DDint_, buf_, dataLength, lineLength, ld.AAAA);
			memset(buf_, 0, 255);
			memset(DDint_, 0, 255);
			memset(DDchar_, 0, 255);
		}
#endif

		lineCount += lineLength;
	}

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("[DWM UPDATE MANAGER] ");
	for (int writingbuf = 0; writingbuf < lineCount; writingbuf++) {
		bbaSerial::dbg_printf("%d ", buff[writingbuf]);
	}
	bbaSerial::dbg_printf("\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		string tmpLog;
		tmpLog.append(StringProcesses::create("[DWM UPDATE MANAGER] "));
		bbaServerLog::dbg_log("[DWM UPDATE MANAGER] ");
		for (int writingbuf = 0; writingbuf < lineCount; writingbuf++) {
			tmpLog.append(StringProcesses::create("%s ", buff[writingbuf]));
		}
		bbaServerLog::dbg_log(tmpLog.c_str());
	}
#endif

	return true;
}

bool bbaNrf52xxBootloader::GetFile(String *File, uint32_t *fileSize,
		const String &path_) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	unsigned long startTime = millis();
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		unsigned long startTime = millis();
	}
#endif

	String URL = path_;

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("[DWM UPDATE MANAGER] %s\n", URL.c_str());
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log("[DWM UPDATE MANAGER] %s", URL.c_str());
	}
#endif

	WiFiClient wificlient;
	HTTPClient httpClient;
	httpClient.begin(wificlient, URL);

	int httpCode = httpClient.GET();

	if (httpCode == 200) {
		*fileSize = httpClient.getSize();

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] File Size : %d\n",
				*fileSize);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[DWM UPDATE MANAGER] File Size : %d",
					*fileSize);
		}
#endif

		*File = httpClient.getString();

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] file : ");
		int i = 0;
		int j = 0;
		for (i = 0; i < *fileSize; i++) {
			if (File->charAt(i) != '\n' || File->charAt(i) != ':') {
				bbaSerial::dbg_printf("%c", File->charAt(i));
			} else {
				bbaSerial::dbg_printf("\tFaund : %c\t", File->charAt(i));
			}
		}
		bbaSerial::dbg_printf("\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log("[DWM UPDATE MANAGER] file : ");
			int i = 0;
			int j = 0;
			for (i = 0; i < *fileSize; i++) {
				if (File->charAt(i) != '\n' || File->charAt(i) != ':') {
					bbaServerLog::dbg_log("%c", File->charAt(i));
				} else {
					bbaServerLog::dbg_log("Faund : %c", File->charAt(i));
				}
			}
		}
#endif
	} else {

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] Error : HTTP Code : %d\n",
				httpCode);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[DWM UPDATE MANAGER] Error : HTTP Code : %d", httpCode);
		}
#endif

		return false;
	}

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("[DWM UPDATE MANAGER] GetFile time : %d\n",
			millis() - startTime);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log("[DWM UPDATE MANAGER] GetFile time : %d",
				millis() - startTime);
	}
#endif

	return true;
}

bool bbaNrf52xxBootloader::checkNewFirmware(String fileUrl) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("[DWM UPDATE MANAGER] Swd Initializing...\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("[DWM UPDATE MANAGER] Swd Initializing...");
	}
#endif

	dap.begin(bbaGlobalSymbols::getDwmSWCLKPin(),
			bbaGlobalSymbols::getDwmSWDIOPin(),
			bbaGlobalSymbols::getDwmSWRSTPin(), &error);

	if (!dap.dap_disconnect()) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf(
				"[DWM UPDATE MANAGER] Bootloader Disconnected: %s\n",
				dap.error_message);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[DWM UPDATE MANAGER] Bootloader Disconnected: %s",
					dap.error_message);
		}
#endif
		return false;
	}

	char debuggername[100];
	if (!dap.dap_get_debugger_info(debuggername)) {
		error(dap.error_message);
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] Debugger Info Error: %s\n",
				debuggername);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[DWM UPDATE MANAGER] Debugger Info Error: %s",
					debuggername);
		}
#endif
	}
	if (!dap.dap_connect()) {
		error(dap.error_message);
		return false;
	}

	if (!dap.dap_transfer_configure(0, 128, 128)) {
		error(dap.error_message);
		return false;
	}
	if (!dap.dap_swd_configure(0)) {
		error(dap.error_message);
		return false;
	}
	if (!dap.dap_reset_link()) {
		error(dap.error_message);
		return false;
	}
	if (!dap.dap_swj_clock(50)) {
		error(dap.error_message);
		return false;
	}
	if (!dap.dap_target_prepare()) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] Target Prepare failed.\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[DWM UPDATE MANAGER] Target Prepare failed.");
		}
#endif
		return false;
	}

	uint32_t dsu_did;
	if (!dap.select(&dsu_did)) {
		error("No nRF5x device found!");
		return false;
	}

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf(
			"[DWM UPDATE MANAGER] Found Target: %s\tFlash size: %d\tFlash pages: %d\tPage size: %d\n\n",
			dap.target_device.name, dap.target_device.flash_size,
			dap.target_device.n_pages, dap.target_device.n_pages);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[DWM UPDATE MANAGER] Found Target: %s\tFlash size: %d\tFlash pages: %d\tPage size: %d",
				dap.target_device.name, dap.target_device.flash_size,
				dap.target_device.flash_size, dap.target_device.n_pages);
	}
#endif
	String file;
	uint32_t filesize = 0;
	if (!GetFile(&file, &filesize, fileUrl)) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] File can not get\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[DWM UPDATE MANAGER] File can not get");
		}
#endif
		return false;
	}

	uint16_t *bufsize;
	if (!stringToBuf(&file, filesize, bufsize)) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
		bbaSerial::dbg_printf("[DWM UPDATE MANAGER] stringToBuf failed\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
		if (bbaLogSymbols::getNrf52xxBootloaderLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[DWM UPDATE MANAGER] stringToBuf failed");
		}
#endif
		return false;
	}

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	string tmpLog;
	tmpLog.append(
			StringProcesses::create("[DWM UPDATE MANAGER] File Content:\n"));
	for (int i = 0; i < bbaGlobalSymbols::getDwmFirmwareBufferSize(); i++) {
		tmpLog.append(StringProcesses::create("%d\t->\t%d", i, buff[i]));
		ESP.wdtFeed();
	}
	bbaSerial::dbg_printf(tmpLog.c_str());
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		string tmpLog;
		tmpLog.append(
				StringProcesses::create(
						"[DWM UPDATE MANAGER] File Content:\t"));
		for (int i = 0; i < bbaGlobalSymbols::getDwmFirmwareBufferSize(); i++) {
			tmpLog.append(StringProcesses::create("%d\t->\t%d", i, buff[i]));
			ESP.wdtFeed();
		}
		bbaServerLog::dbg_log(tmpLog.c_str());
	}
#endif

	file[0] = '\0';

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("Erasing...\n\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("Erasing...");
		return false;
	}
#endif
	dap.erase();
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("Done\n\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("Done");
	}
#endif

	uint32_t start_ms = millis();

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("Programming 32K ...\n\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("Programming 32K ...");
	}
#endif
	dap.program_start();
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("Done\n\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("Done");
	}
#endif

	uint32_t addr = 0;

	for (int i = 0; i < 8; i++) {
		if (!dap.program(addr, buff, sizeof(buff))) {
#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
			bbaSerial::dbg_printf("nRF5x False\n\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
			if (bbaLogSymbols::getNrf52xxBootloaderLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::error_log("nRF5x False");
			}
#endif
		}
		addr += 4096;
	}

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("Done in %d ms\n\n", millis() - start_ms);
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("Done in %d ms", millis() - start_ms);
	}
#endif

	dap.deselect();
	dap.dap_disconnect();
	buff[0] = '\0';

#if ISDEFINED(SERIAL_NRF52XX_BOOTLOADER_LOG)
	bbaSerial::dbg_printf("Update Successfully...\n\n");
#endif
#if ISDEFINED(SERVER_NRF52XX_BOOTLOADER_LOG)
	if (bbaLogSymbols::getNrf52xxBootloaderLog()
			&& bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("Update Successfully...");
	}
#endif

	return true;
}
