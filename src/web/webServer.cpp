/*
 * createServer.cpp
 *
 *  Created on: Dec 29, 2022
 *      Author: aress
 */

#include "webServer.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#include "pages/notFound.h"
#include "pages/test.h"
#include "pages/wifiSetupPortalPost.h"
#include "../utils/eepromUtil.h"
#include "../utils/convertUtil.h"
#include "pages/wifiSetupPortalGetSt.h"
#include "pages/wifiSetupPortalGetAp.h"
#include "pages/wifiSetuportalGetStBody.h"

#include "../utils/symbols/bbaGlobalSymbols.h"
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
#include "../utils/bbaServerLog.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#endif

#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#endif

webServer::webServer() {
}

webServer::~webServer() {
	server.stop();
}

void webServer::begin(uint16_t port) {
	server.on("/", std::bind(&webServer::handle, this));
	server.on("/test", std::bind(&webServer::handleTEST, this));
	server.on("/wifistportal", std::bind(&webServer::handleStPORTAL, this));
	server.on("/wifiapportal", std::bind(&webServer::handleApPORTAL, this));
	server.on("/restart", std::bind(&webServer::restart, this));
	server.onNotFound(std::bind(&webServer::handleNOTFOUND, this));
	server.begin(port);
#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] WEB Server ready! Open http://%s:%d in your browser\n",
			WiFi.localIP().toString().c_str(), port);
#endif
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
	if (bbaLogSymbols::getWebServiceLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log(
				"[UPDATE MANAGER] WEB Server ready! Open http://%s:%d in your browser\n",
				WiFi.localIP().toString().c_str(), port);
	}
#endif
}

void webServer::handle() {
	server.handleClient();
}

void webServer::handleTEST() {
	String page = TEST_page; //Read HTML contents
	server.send(200, "text/html", page); //Send web page
}

void webServer::handleNOTFOUND() {
	String page = NOTFOUND_page; //Read HTML contents
	server.send(404, "text/plain", page); //Send web page
}

void webServer::handleStPORTAL() {
	if (server.method() == HTTP_POST) {
		String page = WIFISETUPPORTALPOST_page;
		std::string ssid = std::string(server.arg("ssid").c_str());
		std::string password = std::string(server.arg("password").c_str());
		std::string channel = std::string(server.arg("channel").c_str());
		std::string hidden = std::string(server.arg("hidden").c_str());
		std::string maxcount = std::string(server.arg("maxcount").c_str());
#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
		bbaSerial::dbg_printf(
				"[UPDATE MANAGER] New SSID: %s, PASSWORD: %s, Channel: %s, Hidden: %s, Max Count: %s\n",
				ssid.c_str(), password.c_str(), channel.c_str(), hidden.c_str(),
				maxcount.c_str());
#endif
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
		if (bbaLogSymbols::getWebServiceLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log(
					"[UPDATE MANAGER] New SSID: %s, PASSWORD: %s, Channel: %s, Hidden: %s, Max Count: %s\n",
					ssid.c_str(), password.c_str(), channel.c_str(),
					hidden.c_str(), maxcount.c_str());
		}
#endif
		updateSTAInfo(ssid, password, channel, hidden, maxcount);
		server.send(200, "text/html", page);
	} else {
		String page =
				"<!doctype html> <html lang='en'>	<head>		<meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1'>			<title>Wifi Setup</title> 			<style>*,::after,::before{box-sizing:border-box;}				body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}				.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}				button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}				.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}				h1{text-align: center}			</style> 	</head> 	<body>		<main class='form-signin'> 			<form action='/wifistportal' method='post'> 				<h1 class=''>Wifi Setup</h1>";
		int n = WiFi.scanNetworks();
		for (int i = 0; i < n; ++i) {
			page +=
					"<div class='form-floating' style='border: 1px solid blue; text-align: center; width: fit-content; height: fit-content; word-wrap: break-word;'><table><label>";
			page += (i + 1);
			page += " - ";
			page += "SSID:";
			page += WiFi.SSID(i);
			page += " RSSI:";
			page += WiFi.RSSI(i);
			page += WiFi.isHidden(i) ? " Hidden" : " NotHidden";
			page += " BSSID:";
			page += WiFi.BSSIDstr(i);
			page += " Ch:";
			page += String(WiFi.channel(i));
			page += " Encrypt:";
			page += bbaWifiManager::encryptStr(WiFi.encryptionType(i)).c_str();
			page += "</label></table></div>";
		}

		page +=
				"<br/>				<div class='form-floating'>					<label>SSID</label>					<input type='text' class='form-control' name='ssid'> 				</div>				<div class='form-floating'>					<br/>					<label>Password</label>					<input type='password' class='form-control' name='password'>				</div>				<div class='form-floating'>					<br/>					<label>Channel</label>					<input type='text' class='form-control' name='channel'>				</div>				<div class='form-floating'>					<br/>					<label>Hidden</label>					<input type='text' class='form-control' name='hidden'>				</div>				<div class='form-floating'>					<br/>					<label>Max Count</label>					<input type='text' class='form-control' name='maxcount'>				</div>				<br/><br/>				<button type='submit'>Save</button>				<p style='text-align: right'><a href='/' style='color: #32C5FF'>Sing Up</a></p>			</form>		</main> 	</body></html>";
		bbaSerial::dbg_printf("4\n");
		server.send(200, "text/html", page);
	}
}

void webServer::handleApPORTAL() {
	if (server.method() == HTTP_POST) {
		String page = WIFISETUPPORTALPOST_page;
		std::string ssid = std::string(server.arg("ssid").c_str());
		std::string password = std::string(server.arg("password").c_str());
		std::string channel = std::string(server.arg("channel").c_str());
		std::string hidden = std::string(server.arg("hidden").c_str());
		std::string maxcount = std::string(server.arg("maxcount").c_str());
#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
		bbaSerial::dbg_printf(
				"[UPDATE MANAGER] New SSID: %s, PASSWORD: %s, Channel: %s, Hidden: %s, Max Count: %s\n",
				ssid.c_str(), password.c_str(), channel.c_str(), hidden.c_str(),
				maxcount.c_str());
#endif
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
		if (bbaLogSymbols::getWebServiceLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::dbg_log(
					"[UPDATE MANAGER] New SSID: %s, PASSWORD: %s, Channel: %s, Hidden: %s, Max Count: %s\n",
					ssid.c_str(), password.c_str(), channel.c_str(),
					hidden.c_str(), maxcount.c_str());
		}
#endif
		updateApInfo(ssid, password, channel, hidden, maxcount);
		server.send(200, "text/html", page);
	} else {
		String page = WIFISETUPPORTALGETAP_page;
		server.send(200, "text/html", page);
	}
}

void webServer::restart() {
	ESP.restart();
}

bool webServer::updateSTAInfo(std::string ssid, std::string password,
		std::string channel, std::string hidden, std::string maxcount) {
	int ichannel = convertUtil::stringToInt(channel);
	bool bhidden = convertUtil::stringToBool(hidden);
	int imaxcount = convertUtil::stringToInt(maxcount);
	bool returnValue = true;
#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] Writing SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
			ssid.c_str(), password.c_str(), ichannel,
			bhidden ? "true" : "false", imaxcount);
#endif
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
	if (bbaLogSymbols::getWebServiceLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[UPDATE MANAGER] Writing SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
				ssid.c_str(), password.c_str(), ichannel,
				bhidden ? "true" : "false", imaxcount);
	}
#endif

	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::STA_SSID, ssid,
				35);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::STA_PASSWORD,
				password, 35);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::STA_CHANNEL,
				ichannel);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::STA_HIDDEN,
				eepromUtil::BITADDRESS::STA_HIDDEN_BIT, bhidden);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::STA_MAXCONN,
				imaxcount);
	}
#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] Verify SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
			eepromUtil::readString(eepromUtil::WIFIADRESS::STA_SSID, 35).c_str(),
			eepromUtil::readString(eepromUtil::WIFIADRESS::STA_PASSWORD, 35).c_str(),
			eepromUtil::readInt(eepromUtil::WIFIADRESS::STA_CHANNEL),
			eepromUtil::readbool(eepromUtil::WIFIADRESS::STA_HIDDEN,
					eepromUtil::BITADDRESS::STA_HIDDEN_BIT) ? "true" : "false",
			eepromUtil::readInt(eepromUtil::WIFIADRESS::STA_MAXCONN));
#endif
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
	if (bbaLogSymbols::getWebServiceLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[UPDATE MANAGER] Verify SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
				eepromUtil::readString(eepromUtil::WIFIADRESS::STA_SSID, 35).c_str(),
				eepromUtil::readString(eepromUtil::WIFIADRESS::STA_PASSWORD, 35).c_str(),
				eepromUtil::readInt(eepromUtil::WIFIADRESS::STA_CHANNEL),
				eepromUtil::readbool(eepromUtil::WIFIADRESS::STA_HIDDEN,
						eepromUtil::BITADDRESS::STA_HIDDEN_BIT) ?
						"true" : "false",
				eepromUtil::readInt(eepromUtil::WIFIADRESS::STA_MAXCONN));
	}
#endif

	return returnValue;
}

bool webServer::updateApInfo(std::string ssid, std::string password,
		std::string channel, std::string hidden, std::string maxcount) {
	int ichannel = convertUtil::stringToInt(channel);
	bool bhidden = convertUtil::stringToBool(hidden);
	int imaxcount = convertUtil::stringToInt(maxcount);
	bool returnValue = true;
#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] Writing SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
			ssid.c_str(), password.c_str(), ichannel,
			bhidden ? "true" : "false", imaxcount);
#endif
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
	if (bbaLogSymbols::getWebServiceLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[UPDATE MANAGER] Writing SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
				ssid.c_str(), password.c_str(), ichannel,
				bhidden ? "true" : "false", imaxcount);
	}
#endif

	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::AP_SSID, ssid,
				35);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::AP_PASSWORD,
				password, 35);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::AP_CHANNEL,
				ichannel);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::AP_HIDDEN,
				eepromUtil::BITADDRESS::AP_HIDDEN_BIT, bhidden);
	}
	if (returnValue) {
		returnValue = eepromUtil::write(eepromUtil::WIFIADRESS::AP_MAXCONN,
				imaxcount);
	}
#if ISDEFINED(SERIAL_WEB_SERVICE_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] Readed SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
			eepromUtil::readString(eepromUtil::WIFIADRESS::AP_SSID, 35).c_str(),
			eepromUtil::readString(eepromUtil::WIFIADRESS::AP_PASSWORD, 35).c_str(),
			eepromUtil::readInt(eepromUtil::WIFIADRESS::AP_CHANNEL),
			eepromUtil::readbool(eepromUtil::WIFIADRESS::AP_HIDDEN,
					eepromUtil::BITADDRESS::AP_HIDDEN_BIT) ? "true" : "false",
			eepromUtil::readInt(eepromUtil::WIFIADRESS::AP_MAXCONN));
#endif
#if ISDEFINED(SERVER_WEB_SERVICE_LOG)
	if (bbaLogSymbols::getWebServiceLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[UPDATE MANAGER] Readed SSID: %s, PASSWORD: %s, Channel: %d, Hidden: %s, Max Count: %d\n",
				eepromUtil::readString(eepromUtil::WIFIADRESS::AP_SSID, 35).c_str(),
				eepromUtil::readString(eepromUtil::WIFIADRESS::AP_PASSWORD, 35).c_str(),
				eepromUtil::readInt(eepromUtil::WIFIADRESS::AP_CHANNEL),
				eepromUtil::readbool(eepromUtil::WIFIADRESS::AP_HIDDEN,
						eepromUtil::BITADDRESS::AP_HIDDEN_BIT) ?
						"true" : "false",
				eepromUtil::readInt(eepromUtil::WIFIADRESS::AP_MAXCONN));
	}
#endif

	return returnValue;
}
