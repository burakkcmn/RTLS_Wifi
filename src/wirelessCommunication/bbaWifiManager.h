/*
 * bbaWifiManager.h
 *
 *  Created on: Jan 16, 2022
 *      Author: aress
 */

#ifndef BBAWIFIMANAGER_H_
#define BBAWIFIMANAGER_H_

#include <Ticker.h>
#include <ESP8266WiFi.h>

class bbaWifiManager {
public:
	static void getInfo();
	static void WifiLoad(bool);
	static std::string getMac();
	static int scanNetwork();
	static std::string encryptStr(const uint8_t &typeNo);
	static bool isConnected();

	struct WIFIINFO {
		String SSID;
		String PASSWORD;
		uint8_t CHANNEL;
		bool ISHIDDEN;
		uint8_t MAXCONN;
	};

	static bbaWifiManager::WIFIINFO STAINFO1;
	static bbaWifiManager::WIFIINFO APINFO1;
	static WiFiEventHandler stationModeConnectedHandler;
	static WiFiEventHandler stationModeDisconnectedHandler;
	static WiFiEventHandler stationModeAuthModeChangedHandler;
	static WiFiEventHandler stationModeGotIPHandler;
	static WiFiEventHandler stationModeDHCPTimeoutHandler;
	static WiFiEventHandler softAPModeStationConnectedHandler;
	static WiFiEventHandler softAPModeStationDisconnectedHandler;
	static WiFiEventHandler softAPModeProbeRequestReceivedHandler;
	static WiFiEventHandler onWiFiModeChangeHandler;
	static int ONBOARDLED;
	static bool wifiLedActive;
	static bool isconnected;

private:

	static void initWifiLed();
	static void initHandler();
	static void reconnectWiFi();
	static bool ConnectWifi();
	static std::string connectionStatus(int);
	static bool isAutoConnected();
	static bool hasWifiConfigChanged();

	static void WiFiEvent(WiFiEvent_t);
	static void onProbeRequestPrint(
			const WiFiEventSoftAPModeProbeRequestReceived &evt);
	static void onStationConnected(const WiFiEventStationModeConnected &evt);
	static void onStationDisconnected(
			const WiFiEventStationModeDisconnected &evt);
	static void onAuthModeChanged(
			const WiFiEventStationModeAuthModeChanged &evt);
	static void onGotIP(const WiFiEventStationModeGotIP &evt);
	static void onDHCPTimeout(void);
	static void onAPModeStationConnected(
			const WiFiEventSoftAPModeStationConnected &evt);
	static void onAPModeStationDisconnected(
			const WiFiEventSoftAPModeStationDisconnected &evt);

};

#endif /* BBAWIFIMANAGER_H_ */
