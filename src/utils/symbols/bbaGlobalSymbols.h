/*
 * bbaGlobalSymbols.h
 *
 *  Created on: Feb 17, 2023
 *      Author: aress
 */

#ifndef UTILS_SYMBOLS_BBAGLOBALSYMBOLS_H_
#define UTILS_SYMBOLS_BBAGLOBALSYMBOLS_H_

#include <string>
#include "bbaLogSymbols.h"

class bbaGlobalSymbols final {
private:
	/*
	 *	ESP VERSION INFORMATIONS
	 */
	static std::string versionEspDevice;
	static std::string versionEspDate;
	static std::string versionEspMajor;
	static std::string versionEspMinor;

	/*
	 *	SERVER INFORMATIONS
	 */
	static std::string serverIpAddress;
	static uint16_t serverUdpPort;
	static uint16_t espUdpPort;
	static uint16_t espHeartbeatPort;

	/*
	 *	ESP WEB INFORMATIONS
	 */
	static int webServicePort;

	/*
	 *	ESP SERIAL INFORMATIONS
	 */
	static uint8_t espRxPin;
	static uint8_t espTxPin;

	static int serialBaudrate;
	static const int maxSerialBufferSize = 256;

	/*
	 *	DWM INFORMATIONS
	 */
	static int anchorId;
	static std::string anchorChipId;
	static std::string anchorBluetoothId;
	static std::string anchorDwmVersion;

	static std::string tagLocation;
	static std::string tagErrorlog;
	static std::string tagInfoLog;
	static std::string tagRegisterInfo;
	static std::string tagChipId;
	static std::string tagBluetoothId;
	static std::string tagVersion;
	static std::string tagRestart;

	/*
	 * Server Commands
	 */
	static std::string tagAnchorIdChange;
	static std::string tagTagIdChange;
	static std::string tagNewTagId;
	static std::string tagAnchorWifiLedBlink;
	static std::string tagAnchorWifiLedOn;
	static std::string tagStopRunning;
	static std::string tagRegisterRequest;

	static bool isRunning;

	/*
	 *	PACKET INFORMATION
	 */
	static std::string bbaGlobalSeperator;
	static std::string bbaGlobalConnector;

	/*
	 *	COMMUNICATION REQUESTS
	 */
	static std::string registerRequest;
	static std::string lastErrorRequest;
	static std::string startMesureRequest;
	static bool dwmRegistered;

	/*
	 *	ESP UPDATE INFORMATIONS
	 */
	static std::string espFirmwareIp;
	static int espFirmwarePort;
	static std::string espFirmwareUrl;

	/*
	 *	ESP WIFI INFORMATIONS
	 */
	static uint8_t espWifiLed;
	static bool espWifiLedOn;

	static bool espWifiLedBlink;

	/*
	 *	DWM UPDATE INFORMATIONS
	 */
	static std::string dwmFirmwareIp;
	static int dwmFirmwarePort;
	static std::string dwmFirmwareUrl;
	static int dwmFirmwareBufferSize;
	static int dwmSWDIOPin;
	static int dwmSWCLKPin;
	static int dwmSWRSTPin;

public:
	static std::string getVersionDevice();
	static std::string getVersionDate();
	static std::string getVersionMajor();
	static std::string getVersionMinor();

	static std::string getVersion();
	static std::string getServerIpAddress();
	static uint16_t getServerUdpPortNumber();
	static uint16_t getESPUdpPortNumber();
	static uint16_t getHeartbeatNumber();
	static int getSerialBaudrrate();
	static int getWebServicePort();
	static int getSerialMaxBufferSize();
	static int getAnchorId();
	static std::string getAnchorChipId();
	static std::string getAnchorBluetoothId();
	static std::string getAnchorDwmVersion();
	static void setAnchorId(const int&);
	static void setAnchorChipId(const std::string&);
	static void setAnchorBluetoothId(const std::string&);
	static void setAnchorDwmVersion(const std::string&);

	static std::string getRegistrationRequest();
	static std::string getLastErrorRequest();
	static std::string getStartMesurerequest();
	static bool isDwmRegistered();
	static void setDwmRegistered(const bool &value);

	static std::string getGlobalSeperator();
	static std::string getGlobalConnector();

	static std::string getLocationTag();
	static std::string getErrorlogTag();
	static std::string getInfoLogTag();
	static std::string getRegisterInfoTag();
	static std::string getChipIdTag();
	static std::string getBluetoothIdTag();
	static std::string getVersionTag();
	static std::string getRestartTag();

	/*
	 * Server Commands
	 */
	static std::string getAnchorIdChangeTag();
	static std::string getTagIdChangeTag();
	static std::string getNewTagIdTag();
	static std::string getAnchorWifiLedBlinkTag();
	static std::string getAnchorWifiLedOnTag();
	static std::string getStopRunningTag();
	static std::string getRegisterRequestTag();

	static std::string getEspFirmwareIp();
	static int getEspFirmwarePort();
	static std::string getEspFirmwareUrl();

	static std::string getDwmFirmwarePath();
	static std::string getDwmFirmwareIp();
	static int getDwmFirmwarePort();
	static std::string getDwmFirmwareUrl();

	static void setEspFirmwareIp(const std::string &ip);
	static void setEspFirmwarePort(const int &port);
	static void setEspFirmwareUrl(const std::string &url);

	static void setDwmFirmwareIp(const std::string &ip);
	static void setDwmFirmwarePort(const int &port);
	static void setDwmFirmwareUrl(const std::string &url);

	static uint8_t getEspRxPin();
	static uint8_t getEspTxPin();

	static uint16_t getDwmFirmwareBufferSize();
	static uint8_t getDwmSWDIOPin();
	static uint8_t getDwmSWCLKPin();
	static uint8_t getDwmSWRSTPin();

	static uint8_t getWifiLedPin();
	static uint8_t isWifiLedOn();
	static void setWifiLedOn(const bool &value);
	static uint8_t isWifiLedBlink();
	static void setWifiLedBlink(const bool &value);

	static void setIsRunning(const bool &value);
	static bool getIsRunning();
};

#endif /* UTILS_SYMBOLS_BBAGLOBALSYMBOLS_H_ */
