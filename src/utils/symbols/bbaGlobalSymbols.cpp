/*
 * bbaGlobalSymbols.cpp
 *
 *  Created on: Feb 17, 2023
 *      Author: aress
 */

#include "bbaGlobalSymbols.h"

#include "../StringProcesses.h"

std::string bbaGlobalSymbols::versionEspDevice = "esp";
std::string bbaGlobalSymbols::versionEspDate = "230923";
std::string bbaGlobalSymbols::versionEspMajor = "0";
std::string bbaGlobalSymbols::versionEspMinor = "26";

std::string bbaGlobalSymbols::serverIpAddress = "13.131.31.25";
uint16_t bbaGlobalSymbols::serverUdpPort = 29700;
uint16_t bbaGlobalSymbols::espUdpPort = 5090;
uint16_t bbaGlobalSymbols::espHeartbeatPort = 27900;
int bbaGlobalSymbols::webServicePort = 80;

int bbaGlobalSymbols::anchorId = 1;
std::string bbaGlobalSymbols::anchorChipId = "1000000000000000";
std::string bbaGlobalSymbols::anchorBluetoothId = "0000000000000000";
std::string bbaGlobalSymbols::anchorDwmVersion = "dwm.230809.0.0";

std::string bbaGlobalSymbols::bbaGlobalSeperator = ",";
std::string bbaGlobalSymbols::bbaGlobalConnector = ":";

std::string bbaGlobalSymbols::registerRequest = "#Register information$";
std::string bbaGlobalSymbols::lastErrorRequest = "#Last Error$";
std::string bbaGlobalSymbols::startMesureRequest = "#Start Mesure$";
bool bbaGlobalSymbols::dwmRegistered = false;

std::string bbaGlobalSymbols::tagLocation = "LOCATION";

std::string bbaGlobalSymbols::tagErrorlog = "ERRORLOG";
std::string bbaGlobalSymbols::tagInfoLog = "INFOLOG";
std::string bbaGlobalSymbols::tagRegisterInfo = "REGISTERINFO";
std::string bbaGlobalSymbols::tagChipId = "ChId";
std::string bbaGlobalSymbols::tagBluetoothId = "BlId";
std::string bbaGlobalSymbols::tagVersion = "V";
/*
 * Server Commands
 */
std::string bbaGlobalSymbols::tagRestart = "esprestart";
std::string bbaGlobalSymbols::tagAnchorIdChange = "AnchorId";
std::string bbaGlobalSymbols::tagTagIdChange = "TagId";
std::string bbaGlobalSymbols::tagNewTagId = "newtagid";
std::string bbaGlobalSymbols::tagAnchorWifiLedBlink = "anchor-wifi-led-blink";
std::string bbaGlobalSymbols::tagAnchorWifiLedOn = "anchor-wifi-led-on";
std::string bbaGlobalSymbols::tagStopRunning = "stoprunning";
std::string bbaGlobalSymbols::tagRegisterRequest = "registerrequest";

bool bbaGlobalSymbols::isRunning = true;

/*
 *	ESP UPDATE INFORMATIONS
 */
std::string bbaGlobalSymbols::espFirmwareIp = "13.131.31.25";
int bbaGlobalSymbols::espFirmwarePort = 7000;
std::string bbaGlobalSymbols::espFirmwareUrl = "/downloadFirmware/esp/";

/*
 *	ESP WIFI INFORMATIONS
 */
uint8_t bbaGlobalSymbols::espWifiLed = 2;
bool bbaGlobalSymbols::espWifiLedOn = false;
bool bbaGlobalSymbols::espWifiLedBlink = false;

/*
 *	DWM UPDATE INFORMATIONS
 */
std::string bbaGlobalSymbols::dwmFirmwareIp = "13.131.31.25";
int bbaGlobalSymbols::dwmFirmwarePort = 8080;
std::string bbaGlobalSymbols::dwmFirmwareUrl = "/downloadFirmware/dwm/";
int bbaGlobalSymbols::dwmFirmwareBufferSize = 4096;
int bbaGlobalSymbols::dwmSWDIOPin = 5;
int bbaGlobalSymbols::dwmSWCLKPin = 4;
int bbaGlobalSymbols::dwmSWRSTPin = 12;

/*
 *	ESP SERIAL INFORMATIONS
 */
uint8_t bbaGlobalSymbols::espRxPin = 13;
uint8_t bbaGlobalSymbols::espTxPin = 15;
int bbaGlobalSymbols::serialBaudrate = 115200;

std::string bbaGlobalSymbols::getVersionDevice() {
	return versionEspDevice;
}

std::string bbaGlobalSymbols::getVersionDate() {
	return versionEspDate;
}

std::string bbaGlobalSymbols::getVersionMajor() {
	return versionEspMajor;
}

std::string bbaGlobalSymbols::getVersionMinor() {
	return versionEspMinor;
}

std::string bbaGlobalSymbols::getVersion() {
	std::string version = versionEspDevice;
	StringProcesses::add(version, ".");
	StringProcesses::add(version, versionEspDate);
	StringProcesses::add(version, ".");
	StringProcesses::add(version, versionEspMajor);
	StringProcesses::add(version, ".");
	StringProcesses::add(version, versionEspMinor);
	return version;
}

std::string bbaGlobalSymbols::getServerIpAddress() {
	return serverIpAddress;
}

uint16_t bbaGlobalSymbols::getServerUdpPortNumber() {
	return serverUdpPort;
}

uint16_t bbaGlobalSymbols::getESPUdpPortNumber() {
	return espUdpPort;
}

uint16_t bbaGlobalSymbols::getHeartbeatNumber() {
	return espHeartbeatPort;
}

int bbaGlobalSymbols::getSerialBaudrrate() {
	return serialBaudrate;
}

int bbaGlobalSymbols::getWebServicePort() {
	return webServicePort;
}

int bbaGlobalSymbols::getSerialMaxBufferSize() {
	return maxSerialBufferSize;
}

int bbaGlobalSymbols::getAnchorId() {
	return anchorId;
}

std::string bbaGlobalSymbols::getAnchorChipId() {
	return anchorChipId;
}

std::string bbaGlobalSymbols::getAnchorBluetoothId() {
	return anchorBluetoothId;
}

std::string bbaGlobalSymbols::getAnchorDwmVersion() {
	return anchorDwmVersion;
}

void bbaGlobalSymbols::setAnchorId(const int &anchorid) {
	anchorId = anchorid;
}

void bbaGlobalSymbols::setAnchorChipId(const std::string &anchorchipid) {
	anchorChipId = anchorchipid;
}

void bbaGlobalSymbols::setAnchorBluetoothId(
		const std::string &anchorbluetoothid) {
	anchorBluetoothId = anchorbluetoothid;
}

void bbaGlobalSymbols::setAnchorDwmVersion(
		const std::string &anchordwmversion) {
	anchorDwmVersion = anchordwmversion;
}

std::string bbaGlobalSymbols::getRegistrationRequest() {
	return registerRequest;
}

std::string bbaGlobalSymbols::getLastErrorRequest() {
	return lastErrorRequest;
}

std::string bbaGlobalSymbols::getStartMesurerequest() {
	return startMesureRequest;
}

bool bbaGlobalSymbols::isDwmRegistered() {
	return dwmRegistered;
}
void bbaGlobalSymbols::setDwmRegistered(const bool &value) {
	dwmRegistered = value;
}

std::string bbaGlobalSymbols::getGlobalSeperator() {
	return bbaGlobalSeperator;
}

std::string bbaGlobalSymbols::getGlobalConnector() {
	return bbaGlobalConnector;
}

std::string bbaGlobalSymbols::getLocationTag() {
	return tagLocation;
}

std::string bbaGlobalSymbols::getErrorlogTag() {
	return tagErrorlog;
}

std::string bbaGlobalSymbols::getInfoLogTag() {
	return tagInfoLog;
}

std::string bbaGlobalSymbols::getRegisterInfoTag() {
	return tagRegisterInfo;
}

std::string bbaGlobalSymbols::getChipIdTag() {
	return tagChipId;
}

std::string bbaGlobalSymbols::getBluetoothIdTag() {
	return tagBluetoothId;
}

std::string bbaGlobalSymbols::getVersionTag() {
	return tagVersion;
}

std::string bbaGlobalSymbols::getRestartTag() {
	return tagRestart;
}

std::string bbaGlobalSymbols::getAnchorIdChangeTag() {
	return tagAnchorIdChange;
}

std::string bbaGlobalSymbols::getTagIdChangeTag() {
	return tagTagIdChange;
}

std::string bbaGlobalSymbols::getNewTagIdTag() {
	return tagNewTagId;
}

std::string bbaGlobalSymbols::getAnchorWifiLedBlinkTag() {
	return tagAnchorWifiLedBlink;
}

std::string bbaGlobalSymbols::getAnchorWifiLedOnTag() {
	return tagAnchorWifiLedOn;
}

std::string bbaGlobalSymbols::getStopRunningTag() {
	return tagStopRunning;
}

std::string bbaGlobalSymbols::getRegisterRequestTag() {
	return tagRegisterRequest;
}

std::string bbaGlobalSymbols::getEspFirmwareIp() {
	return espFirmwareIp;
}

int bbaGlobalSymbols::getEspFirmwarePort() {
	return espFirmwarePort;
}

std::string bbaGlobalSymbols::getEspFirmwareUrl() {
	return espFirmwareUrl + getVersion();
}

std::string bbaGlobalSymbols::getDwmFirmwarePath() {
	string retval = StringProcesses::create( //
			"%s:%d%s%s",//
			getDwmFirmwareIp().c_str(), //
			getDwmFirmwarePort(), //
			getDwmFirmwareUrl().c_str(), //
			getAnchorDwmVersion().c_str() //
			);
	return retval;
}

std::string bbaGlobalSymbols::getDwmFirmwareIp() {
	return dwmFirmwareIp;
}

int bbaGlobalSymbols::getDwmFirmwarePort() {
	return dwmFirmwarePort;
}

std::string bbaGlobalSymbols::getDwmFirmwareUrl() {
	return dwmFirmwareUrl;
}

void bbaGlobalSymbols::setEspFirmwareIp(const std::string &ip) {
	espFirmwareIp = ip;
}

void bbaGlobalSymbols::setEspFirmwarePort(const int &port) {
	espFirmwarePort = port;
}

void bbaGlobalSymbols::setEspFirmwareUrl(const std::string &url) {
	espFirmwareUrl = url;
}

void bbaGlobalSymbols::setDwmFirmwareIp(const std::string &ip) {
	dwmFirmwareIp = ip;
}

void bbaGlobalSymbols::setDwmFirmwarePort(const int &port) {
	dwmFirmwarePort = port;
}

void bbaGlobalSymbols::setDwmFirmwareUrl(const std::string &url) {
	dwmFirmwareUrl = url;
}

uint8_t bbaGlobalSymbols::getEspRxPin() {
	return espRxPin;
}

uint8_t bbaGlobalSymbols::getEspTxPin() {
	return espTxPin;
}

uint16_t bbaGlobalSymbols::getDwmFirmwareBufferSize() {
	return dwmFirmwareBufferSize;
}

uint8_t bbaGlobalSymbols::getDwmSWDIOPin() {
	return dwmSWDIOPin;
}

uint8_t bbaGlobalSymbols::getDwmSWCLKPin() {
	return dwmSWCLKPin;
}

uint8_t bbaGlobalSymbols::getDwmSWRSTPin() {
	return dwmSWRSTPin;
}

uint8_t bbaGlobalSymbols::getWifiLedPin() {
	return espWifiLed;
}

uint8_t bbaGlobalSymbols::isWifiLedOn() {
	return espWifiLedOn;
}

void bbaGlobalSymbols::setWifiLedOn(const bool &value) {
	espWifiLedOn = value;
}

uint8_t bbaGlobalSymbols::isWifiLedBlink() {
	return espWifiLedBlink;
}

void bbaGlobalSymbols::setWifiLedBlink(const bool &value) {
	espWifiLedBlink = value;
}

void bbaGlobalSymbols::setIsRunning(const bool &value) {
	isRunning = value;
}

bool bbaGlobalSymbols::getIsRunning() {
	return isRunning;
}
