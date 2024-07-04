/*
 * bbaWifiManager.cpp
 *
 *  Created on: Jan 16, 2022
 *      Author: aress
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiType.h>
#include "bbaWifiManager.h"
#include "../utils/eepromUtil.h"
#include "../utils/convertUtil.h"

#include "../utils/symbols/bbaLogSymbols.h"

#if ISDEFINED(SERVER_WIFI_LOG)
#include "../utils/bbaServerLog.h"
#endif

#if ISDEFINED(SERIAL_WIFI_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#endif

#if ISDEFINED(SERVER_WIFI_LOG) || ISDEFINED(SERIAL_WIFI_LOG)
#include "../utils/StringProcesses.h"
#endif

bbaWifiManager::WIFIINFO bbaWifiManager::STAINFO1;
bbaWifiManager::WIFIINFO bbaWifiManager::APINFO1;

int bbaWifiManager::ONBOARDLED = 2;
bool bbaWifiManager::wifiLedActive = false;

WiFiEventHandler bbaWifiManager::stationModeConnectedHandler;
WiFiEventHandler bbaWifiManager::stationModeDisconnectedHandler;
WiFiEventHandler bbaWifiManager::stationModeAuthModeChangedHandler;
WiFiEventHandler bbaWifiManager::stationModeGotIPHandler;
WiFiEventHandler bbaWifiManager::stationModeDHCPTimeoutHandler;
WiFiEventHandler bbaWifiManager::softAPModeStationConnectedHandler;
WiFiEventHandler bbaWifiManager::softAPModeStationDisconnectedHandler;
WiFiEventHandler bbaWifiManager::softAPModeProbeRequestReceivedHandler;
WiFiEventHandler bbaWifiManager::onWiFiModeChangeHandler;
bool bbaWifiManager::isconnected = false;

void bbaWifiManager::getInfo() {
	//STA Info
	STAINFO1.SSID = "FiberHGW_TPFBEA_2.4GHz";
//			eepromUtil::readString(eepromUtil::WIFIADRESS::STA_SSID, 35).c_str();
	STAINFO1.PASSWORD = "e39u7nWt";
//			eepromUtil::readString(			eepromUtil::WIFIADRESS::STA_PASSWORD, 35).c_str();
	STAINFO1.CHANNEL = eepromUtil::readInt(eepromUtil::WIFIADRESS::STA_CHANNEL);
	STAINFO1.ISHIDDEN = eepromUtil::readbool(eepromUtil::WIFIADRESS::STA_HIDDEN,
			eepromUtil::BITADDRESS::STA_HIDDEN_BIT);
	STAINFO1.MAXCONN = eepromUtil::readInt(eepromUtil::WIFIADRESS::STA_MAXCONN);
	// AP Info
	APINFO1.SSID = "bbaWIFI";
	//eepromUtil::readString(eepromUtil::WIFIADRESS::AP_SSID, 35).c_str();
	APINFO1.PASSWORD = "bbaWIFI123.*";
	//eepromUtil::readString(eepromUtil::WIFIADRESS::AP_PASSWORD, 35).c_str();
	APINFO1.CHANNEL = 1; //eepromUtil::readInt(eepromUtil::WIFIADRESS::AP_CHANNEL);
	APINFO1.ISHIDDEN = false; //eepromUtil::readbool(eepromUtil::WIFIADRESS::AP_HIDDEN,eepromUtil::BITADDRESS::AP_HIDDEN_BIT);
	APINFO1.MAXCONN = 50; //eepromUtil::readInt(eepromUtil::WIFIADRESS::AP_MAXCONN);

#if ISDEFINED(SERIAL_WIFI_LOG)
	string logString;
	if (bbaLogSymbols::getGpioControllerLog()) {
		logString = "STA MODE WIFI INFO:	";
		logString.append(
				StringProcesses::create("SSID: %s	-	", STAINFO1.SSID.c_str()));
		logString.append(
				StringProcesses::create("PASSWORD: %s	-	",
						STAINFO1.PASSWORD.c_str()));
		logString.append(
				StringProcesses::create("CHANNEL: %d	-	", STAINFO1.CHANNEL));
		logString.append(
				StringProcesses::create("IS HIDDEN: %s	-	",
						STAINFO1.ISHIDDEN == true ? "true" : "false"));
		logString.append(
				StringProcesses::create("MAX COUNT: %d\n", STAINFO1.MAXCONN));
		logString.append(StringProcesses::create("AP MODE WIFI INFO:	"));
		logString.append(StringProcesses::create("SSID: %s	-	", APINFO1.SSID));
		logString.append(
				StringProcesses::create("PASSWORD: %s	-	", APINFO1.PASSWORD));
		logString.append(
				StringProcesses::create("CHANNEL: %d	-	", APINFO1.CHANNEL));
		logString.append(
				StringProcesses::create("IS HIDDEN: %s	-	",
						APINFO1.ISHIDDEN == true ? "true" : "false"));
		logString.append(
				StringProcesses::create("MAX COUNT: %d\n", APINFO1.MAXCONN));

		bbaSerial::dbg_printf("[WIFI] %s", logString.c_str());
	}
#endif
}

void bbaWifiManager::WifiLoad(bool led_on) {
	initHandler();

	getInfo();
	if (led_on) {
		wifiLedActive = led_on;
		initWifiLed();
	}

	WiFi.mode(WIFI_AP_STA);

	WiFi.softAP(APINFO1.SSID, APINFO1.PASSWORD, APINFO1.CHANNEL,
			APINFO1.ISHIDDEN, APINFO1.MAXCONN);

	if (!isAutoConnected() || hasWifiConfigChanged()) {
		if (!ConnectWifi()) {
			ESP.restart();
		}
	}
}

void bbaWifiManager::initHandler() {
//	WiFi.onEvent(bbaWifiManager::WiFiEvent);

	stationModeConnectedHandler = WiFi.onStationModeConnected(
			&bbaWifiManager::onStationConnected);
	stationModeDisconnectedHandler = WiFi.onStationModeDisconnected(
			&bbaWifiManager::onStationDisconnected);

	softAPModeStationConnectedHandler = WiFi.onSoftAPModeStationConnected(
			&bbaWifiManager::onAPModeStationConnected);
	softAPModeStationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(
			&bbaWifiManager::onAPModeStationDisconnected);
	stationModeAuthModeChangedHandler = WiFi.onStationModeAuthModeChanged(
			&bbaWifiManager::onAuthModeChanged);
	stationModeGotIPHandler = WiFi.onStationModeGotIP(&bbaWifiManager::onGotIP);
	stationModeDHCPTimeoutHandler = WiFi.onStationModeDHCPTimeout(
			&bbaWifiManager::onDHCPTimeout);

	softAPModeProbeRequestReceivedHandler =
			WiFi.onSoftAPModeProbeRequestReceived(
					&bbaWifiManager::onProbeRequestPrint);

//	onWiFiModeChangeHandler;
}

void bbaWifiManager::initWifiLed() {
	pinMode(ONBOARDLED, OUTPUT);
	digitalWrite(ONBOARDLED, HIGH);
}

std::string bbaWifiManager::encryptStr(const uint8_t &typeNo) {
	switch (typeNo) {
	case 0:
		return "WIFI_AUTH_OPEN";
	case 1:
		return "WIFI_AUTH_WEP";
	case 2:
		return "WIFI_AUTH_WPA_PSK";
	case 3:
		return "WIFI_AUTH_WPA2_PSK";
	case 4:
		return "WIFI_AUTH_WPA_WPA2_PSK";
	case 5:
		return "WIFI_AUTH_WPA2_ENTERPRISE";
	case 6:
		return "WIFI_AUTH_MAX";
	}
	return "NONE";
}

void bbaWifiManager::onProbeRequestPrint(
		const WiFiEventSoftAPModeProbeRequestReceived &evt) {
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("[WIFI] Probe request from: %s RSSI: %d\n",
			convertUtil::macToString(evt.mac).c_str(), evt.rssi);
#endif
//#if ISDEFINED(SERVER_WIFI_LOG)
//	if (bbaLogSymbols::getGpioControllerLog()
//			&& bbaWifiManager::isConnected()) {
//		bbaServerLog::dbg_log("[WIFI] Probe request from: %s RSSI: %d",
//				convertUtil::macToString(evt.mac).c_str(), evt.rssi);
//	}
//#endif
}

void bbaWifiManager::onStationConnected(
		const WiFiEventStationModeConnected &evt) {
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("[WIFI] Station connected. SSID:%s CHANNEL: %d\n",
			evt.ssid, evt.channel);
#endif
}

void bbaWifiManager::onStationDisconnected(
		const WiFiEventStationModeDisconnected &evt) {
	bbaWifiManager::isconnected = false;
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("[WIFI] Station disconnected. SSID:%s REASON: %d\n",
			evt.ssid, evt.reason);
#endif
	if (evt.reason == WIFI_DISCONNECT_REASON_NO_AP_FOUND) {
	}
}

void bbaWifiManager::onAuthModeChanged(
		const WiFiEventStationModeAuthModeChanged &evt) {
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("[WIFI] Mode changed. Old mode: %d New mode: %d\n",
			evt.oldMode, evt.newMode);
#endif
//#if ISDEFINED(SERVER_WIFI_LOG)
//	if (bbaLogSymbols::getGpioControllerLog()
//			&& bbaWifiManager::isConnected()) {
//		bbaServerLog::dbg_log(
//				"[WIFI] Mode changed. Old mode: %d New mode: %d\n", evt.oldMode,
//				evt.newMode);
//	}
//#endif
}

void bbaWifiManager::onGotIP(const WiFiEventStationModeGotIP &evt) {
	bbaWifiManager::isconnected = true;
#if ISDEFINED(SERIAL_WIFI_LOG)
	char ipStr[16];
	char gwStr[16];
	char maskStr[16];
	evt.ip.toString().toCharArray(ipStr, sizeof(ipStr));
	evt.gw.toString().toCharArray(gwStr, sizeof(gwStr));
	evt.mask.toString().toCharArray(maskStr, sizeof(maskStr));
	bbaSerial::dbg_printf("[WIFI] GotIP: IP: %s Gateway: %s, MASK: %s\n", ipStr,
			gwStr, maskStr);
#endif
//#if ISDEFINED(SERVER_WIFI_LOG)
//	if (bbaLogSymbols::getGpioControllerLog()
//			&& bbaWifiManager::isConnected()) {
//		char ipStr[16];
//		char gwStr[16];
//		char maskStr[16];
//		evt.ip.toString().toCharArray(ipStr, sizeof(ipStr));
//		evt.gw.toString().toCharArray(gwStr, sizeof(gwStr));
//		evt.mask.toString().toCharArray(maskStr, sizeof(maskStr));
//		bbaServerLog::dbg_log("[WIFI] GotIP: IP: %s Gateway: %s, MASK: %s",
//				ipStr, gwStr, maskStr);
//	}
//#endif
}

void bbaWifiManager::onDHCPTimeout(void) {
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("[WIFI] DHCP Timeout\n");
#endif
}

void bbaWifiManager::onAPModeStationConnected(
		const WiFiEventSoftAPModeStationConnected &evt) {
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("[WIFI] Station connected: MAC: %s Aid: %d\n",
			convertUtil::macToString(evt.mac), evt.aid);
#endif
//#if ISDEFINED(SERVER_WIFI_LOG)
//	if (bbaLogSymbols::getGpioControllerLog()
//			&& bbaWifiManager::isConnected()) {
//		bbaServerLog::dbg_log("[WIFI] Station connected: MAC: %s Aid: %d",
//				convertUtil::macToString(evt.mac), evt.aid);
//	}
//#endif
}

void bbaWifiManager::onAPModeStationDisconnected(
		const WiFiEventSoftAPModeStationDisconnected &evt) {
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("[WIFI] Station connected: MAC: %s Aid: %d\n",
			convertUtil::macToString(evt.mac), evt.aid);
#endif
}

bool bbaWifiManager::ConnectWifi() {
#if ISDEFINED(SERIAL_WIFI_LOG)
	bbaSerial::dbg_printf("Connecting...\n");
#endif
	WiFi.begin(STAINFO1.SSID, STAINFO1.PASSWORD, STAINFO1.CHANNEL);

	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(false);
	int connRes = WiFi.waitForConnectResult();
	if (connRes == WL_CONNECTED) {
#if ISDEFINED(SERIAL_WIFI_LOG)
		bbaSerial::dbg_printf("[WiFi] Connected to %s, IP address: ",
				WiFi.SSID().c_str());
		Serial.println(WiFi.localIP());
#endif
		return true;
	} else {
#if ISDEFINED(SERIAL_WIFI_LOG)
		bbaSerial::dbg_printf(
				"[WiFi] Connection to %s failed - Status %d, %s, Retrying in 10s...\n",
				WiFi.SSID().c_str(), WiFi.status(),
				connectionStatus(WiFi.status()).c_str());
#endif
		WiFi.disconnect();
		return false;
	}
}

bool bbaWifiManager::isConnected() {
	return bbaWifiManager::isconnected;
}

std::string bbaWifiManager::connectionStatus(int which) {
	switch (which) {
	case WL_CONNECTED:
		bbaWifiManager::isconnected = true;
		return "Connected";

	case WL_NO_SSID_AVAIL:
		bbaWifiManager::isconnected = false;
		return "Network not availible";

	case WL_CONNECT_FAILED:
		bbaWifiManager::isconnected = false;
		return "Wrong password";
		break;

	case WL_IDLE_STATUS:
		bbaWifiManager::isconnected = false;
		return "Idle status";
		break;

	case WL_DISCONNECTED:
		bbaWifiManager::isconnected = false;
		return "Disconnected";
		break;

	default:
		bbaWifiManager::isconnected = false;
		return "Unknown";
		break;
	}
}

bool bbaWifiManager::isAutoConnected() {
	int connRes = WiFi.waitForConnectResult();
	if (connRes == WL_CONNECTED) {
#if ISDEFINED(SERIAL_WIFI_LOG)
		bbaSerial::dbg_printf("[WiFi] Auto-connected to %s, IP address: ",
				WiFi.SSID().c_str());
		Serial.println(WiFi.localIP());
#endif
		return true;
	} else {
#if ISDEFINED(SERIAL_WIFI_LOG)
		bbaSerial::dbg_printf(
				"[WiFi] Auto-connect to %s failed - Status %d, %s\n",
				WiFi.SSID().c_str(), WiFi.status(),
				connectionStatus(WiFi.status()).c_str());
#endif
		return false;
	}
}

bool bbaWifiManager::hasWifiConfigChanged() {
	if (strcmp(WiFi.SSID().c_str(), STAINFO1.SSID.c_str()) != 0) {
#if ISDEFINED(SERIAL_WIFI_LOG)
		bbaSerial::dbg_printf("[WiFi] Config Changed, Disconnecting...\n");
#endif
//#if ISDEFINED(SERVER_WIFI_LOG)
//		if (bbaLogSymbols::getGpioControllerLog()
//				&& bbaWifiManager::isConnected()) {
//			bbaServerLog::dbg_log("[WIFI] Config Changed, Disconnecting...");
//		}
//#endif
		WiFi.setAutoReconnect(false);
		WiFi.disconnect();
		return true;
	} else {
#if ISDEFINED(SERIAL_WIFI_LOG)
		bbaSerial::dbg_printf(
				"[WIFI] Config can not change, not Disconnecting...\n");
#endif
//#if ISDEFINED(SERVER_WIFI_LOG)
//		if (bbaLogSymbols::getGpioControllerLog()
//				&& bbaWifiManager::isConnected()) {
//			bbaServerLog::dbg_log(
//					"[WIFI] Config can not change, not Disconnecting...\n");
//		}
//#endif
		return false;
	}
}

std::string bbaWifiManager::getMac() {
	return std::string(WiFi.macAddress().c_str(), WiFi.macAddress().length());
}

int bbaWifiManager::scanNetwork() {
	int scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
	if (scanResult == 0) {
		Serial.println(F("No networks found"));
	} else if (scanResult > 0) {
		Serial.printf(PSTR("%d networks found:\n"), scanResult);

		String ssid;
		int32_t rssi;
		uint8_t encryptionType;
		uint8_t *bssid;
		int32_t channel;
		bool hidden;

		// Print unsorted scan results
		for (int8_t i = 0; i < scanResult; i++) {
			WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel,
					hidden);

			String phyMode;
			const char *wps = "";

//			Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %-11s %3S %s\n"), i, channel, bssid[0], bssid[1],
//					bssid[2], bssid[3], bssid[4], bssid[5], rssi, (encryptionType == ENC_TYPE_NONE) ? ' ' : '*', hidden ? 'H' : 'V', phyMode.c_str(),
//					wps, ssid.c_str());
			yield();
		}
	} else {
#if ISDEFINED(SERIAL_WIFI_LOG)
		bbaSerial::dbg_printf("[WIFI] WiFi scan error %d", scanResult);
#endif
	}
	return 1;
}

