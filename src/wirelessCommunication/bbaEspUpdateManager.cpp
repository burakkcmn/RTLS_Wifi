/*
 * bbaEspUploadManager.cpp
 *
 *  Created on: Jan 17, 2022
 *      Author: aress
 */

#include "../utils/StringProcesses.h"
#include "bbaEspUpdateManager.h"

#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
#include "../utils/bbaServerLog.h"
#include "../wirelessCommunication/bbaWifiManager.h"
#endif

#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
#include "../wiredCommunication/serial/bbaSerial.h"
#endif

uint8_t bbaEspUploadManager::count = 0;

void bbaEspUploadManager::OtaConfig(const int &mode/*uploadMode*/) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf("[UPDATE MANAGER] Ota Starts Configure.\n\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("[UPDATE MANAGER] Ota Starts Configure.");
	}
#endif
	switch (mode) {
	case UpdateMode::IDE:
		ArduinoOTA.setPort(otaIdePort);
		uint8_t wifiMac[6];
		WiFi.macAddress(wifiMac);
		char otahostname[255];
		sprintf(otahostname, "%s%02x%02x%02x%02x%02x%02x", otaIdeHostName,
				MAC2STR(wifiMac));

		ArduinoOTA.setHostname(otahostname);
		ArduinoOTA.setPassword(otaIdePass.c_str());

		ArduinoOTA.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH) {
				type = "sketch";
			} else {
				type = "filesystem";
			}
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
			bbaSerial::dbg_printf("[UPDATE MANAGER] Starting update %s\n", type.c_str());
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
			if (bbaLogSymbols::getUpdateManagerLog()
					&& bbaWifiManager::isConnected()) {
				bbaServerLog::info_log("[UPDATE MANAGER] Starting update %s",
						type.c_str());
			}
#endif
		});

		ArduinoOTA.onEnd([]() {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
			bbaSerial::dbg_printf("[UPDATE MANAGER] \nEnd\n\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
			if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
				bbaServerLog::info_log("[UPDATE MANAGER] End");
			}
#endif

		});

		ArduinoOTA.onProgress(
		[](unsigned int progress, unsigned int total) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
			bbaSerial::dbg_printf("[UPDATE MANAGER] Progress: %u%%\r\n",
					(progress / (total / 100)));
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
			if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
				bbaServerLog::dbg_log("[UPDATE MANAGER] Progress: %u%%", (progress / (total / 100)));
			}
#endif

		});

		ArduinoOTA.onError([](ota_error_t error) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
			bbaSerial::dbg_printf("[UPDATE MANAGER] Error[%s]: \n\n", error);
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
			if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
				bbaServerLog::error_log("[UPDATE MANAGER] Error[%s]: ", error);
			}
#endif

			if (error == OTA_AUTH_ERROR) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
				bbaSerial::dbg_printf("[UPDATE MANAGER] Auth Failed\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
				if (bbaLogSymbols::getEepromLog()&& bbaWifiManager::isConnected()) {
					bbaServerLog::error_log("[UPDATE MANAGER] Auth Failed");
				}
#endif

			} else if (error == OTA_BEGIN_ERROR) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
				bbaSerial::dbg_printf("[UPDATE MANAGER] Begin Failed\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
				if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
					bbaServerLog::error_log("[UPDATE MANAGER] Begin Failed");
				}
#endif

			} else if (error == OTA_CONNECT_ERROR) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
				bbaSerial::dbg_printf("[UPDATE MANAGER] Connect Failed\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
				if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
					bbaServerLog::error_log("[UPDATE MANAGER] Connect Failed");
				}
#endif

			} else if (error == OTA_RECEIVE_ERROR) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
				bbaSerial::dbg_printf("[UPDATE MANAGER] Receive Failed\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
				if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
					bbaServerLog::error_log("[UPDATE MANAGER] Receive Failed");
				}
#endif

			} else if (error == OTA_END_ERROR) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
				bbaSerial::dbg_printf("[UPDATE MANAGER] End Failed\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
				if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
					bbaServerLog::error_log("[UPDATE MANAGER] End Failed");
				}
#endif

			}
		});
		break;

		case UpdateMode::WEB:
		MDNS.begin(otaIdeHostName);

		httpServer.begin(80);
		httpUpdater.setup(&httpServer);

		MDNS.addService("http", "tcp", 80);
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf(
		"[UPDATE MANAGER] HTTPUpdateServer ready! Open http://%s/update in your browser\n",
		WiFi.localIP().toString().c_str());
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log("[UPDATE MANAGER] HTTPUpdateServer ready! Open http://%s/update in your browser",
			WiFi.localIP().toString().c_str());
		}
#endif
		break;
		case UpdateMode::HTTP:
		// Add optional callback notifiers
		ESPhttpUpdate.onStart(update_started);
		ESPhttpUpdate.onEnd(update_finished);
		ESPhttpUpdate.onProgress(update_progress);
		ESPhttpUpdate.onError(update_error);

		ESPhttpUpdate.rebootOnUpdate(true);// remove automatic update
		break;
		default:
		break;
	}
	ArduinoOTA.begin();
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf("[UPDATE MANAGER] OTA configured...\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("[UPDATE MANAGER] OTA configured...");
	}
#endif
}

void bbaEspUploadManager::otaHandle() {
	if (WebOta) {
		httpServer.handleClient();
	}
	if (IdeOta) {
		ArduinoOTA.handle();
	}

}

void bbaEspUploadManager::checkForUpdates(const String &_fwUrlBase,
		const String &_fwFileName) {
	String mac = getMAC();
	String fwURL = String(_fwUrlBase);
	fwURL.concat(_fwFileName.c_str());
	String fwVersionURL = fwURL;
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] Checking for firmware updates. MAC address: %s . Firmware version URL: %s.\n",
			_fwFileName, fwVersionURL.c_str());
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[UPDATE MANAGER] Checking for firmware updates. MAC address: %s . Firmware version URL: %s.",
				_fwFileName, fwVersionURL.c_str());
	}
#endif

	WiFiClient wificlient;
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf("[UPDATE MANAGER] Preparing to update.\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log("[UPDATE MANAGER] Preparing to update.");
	}
#endif

	String fwImageURL = fwURL;
	fwImageURL.concat(".bin");
	t_httpUpdate_return ret;
	WiFiClient client;

	switch (ret) {
	case HTTP_UPDATE_FAILED:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf(
				"[UPDATE MANAGER] HTTP_UPDATE_FAILD Error (%d): %s\n",
				ESPhttpUpdate.getLastError(),
				ESPhttpUpdate.getLastErrorString().c_str());
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[UPDATE MANAGER] HTTP_UPDATE_FAILD Error (%d): %s",
					ESPhttpUpdate.getLastError(),
					ESPhttpUpdate.getLastErrorString().c_str());
		}
#endif

		break;

	case HTTP_UPDATE_NO_UPDATES:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf("[UPDATE MANAGER] HTTP_UPDATE_NO_UPDATES\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[UPDATE MANAGER] HTTP_UPDATE_NO_UPDATES");
		}
#endif

		break;
	}
}

void bbaEspUploadManager::httpUpdateNow() {
	WiFiClient client;

	// The line below is optional. It can be used to blink the LED on the board during flashing
	// The LED will be on during download of one buffer of data from the network. The LED will
	// be off during writing that buffer to flash
	// On a good connection the LED should flash regularly. On a bad connection the LED will be
	// on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
	// value is used to put the LED on. If the LED is on with HIGH, that value should be passed
	//ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

	t_httpUpdate_return ret = ESPhttpUpdate.update(
			client, //
			StringProcesses::stdStringToString(
					bbaGlobalSymbols::getEspFirmwareIp()), //
			bbaGlobalSymbols::getEspFirmwarePort(), //
			StringProcesses::stdStringToString(
					bbaGlobalSymbols::getEspFirmwareUrl()), //
			StringProcesses::stdStringToString(bbaGlobalSymbols::getVersion()));
	// Or:
	//t_httpUpdate_return ret = ESPzhttpUpdate.update(client, "server", 80, "file.bin");

	switch (ret) {
	case HTTP_UPDATE_FAILED:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf(
				"[UPDATE MANAGER] HTTP_UPDATE_FAILD Error (%d): %s\n",
				ESPhttpUpdate.getLastError(),
				ESPhttpUpdate.getLastErrorString().c_str());
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[UPDATE MANAGER] HTTP_UPDATE_FAILD Error (%d): %s",
					ESPhttpUpdate.getLastError(),
					ESPhttpUpdate.getLastErrorString().c_str());
		}
#endif
		break;

	case HTTP_UPDATE_NO_UPDATES:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf("[UPDATE MANAGER] HTTP_UPDATE_NO_UPDATES\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[UPDATE MANAGER] HTTP_UPDATE_NO_UPDATES");
		}
#endif
		break;

	case HTTP_UPDATE_OK:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf("[UPDATE MANAGER] HTTP_UPDATE_OK\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::info_log("[UPDATE MANAGER] HTTP_UPDATE_OK");
		}
#endif
		break;
	}
}

void bbaEspUploadManager::httpsUpdateNow() {
	String host = "";
	uint16_t httpsPort = 80;

	WiFiClientSecure client;

	if (!client.connect(host, httpsPort)) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf("[UPDATE MANAGER] Connection failed\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[UPDATE MANAGER] Connection failed");
		}
#endif
		return;
	}

//	if (client.verify(fingerprint, host)) {
//#ifdef uploadmanager_dbg
	//bbaSerial::dbg_printf("certificate matches");
//#endif
//	} else {
//#ifdef uploadmanager_dbg
	//bbaSerial::dbg_printf("certificate doesn't match");
//#endif
//		return;
//	}

	auto ret = ESPhttpUpdate.update(client, host,
			"http://localhost:8080/RTLS/espbin.jsp");

	switch (ret) {
	case HTTP_UPDATE_FAILED:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf(
				"[UPDATE MANAGER] HTTP_UPDATE_FAILD Error (%d): %s\n",
				ESPhttpUpdate.getLastError(),
				ESPhttpUpdate.getLastErrorString().c_str());
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log(
					"[UPDATE MANAGER] HTTP_UPDATE_FAILD Error (%d): %s",
					ESPhttpUpdate.getLastError(),
					ESPhttpUpdate.getLastErrorString().c_str());
		}
#endif
		break;

	case HTTP_UPDATE_NO_UPDATES:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf("[UPDATE MANAGER] HTTP_UPDATE_NO_UPDATES\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[UPDATE MANAGER] HTTP_UPDATE_NO_UPDATES");
		}
#endif
		break;

	case HTTP_UPDATE_OK:
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
		bbaSerial::dbg_printf("[UPDATE MANAGER] HTTP_UPDATE_OK\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
		if (bbaLogSymbols::getUpdateManagerLog()
				&& bbaWifiManager::isConnected()) {
			bbaServerLog::error_log("[UPDATE MANAGER] HTTP_UPDATE_OK\n");
		}
#endif
		break;
	}
}

String bbaEspUploadManager::getMAC() {
	uint8_t mac[6];
	char result[14];

	snprintf(result, sizeof(result), "%02x%02x%02x%02x%02x%02x", mac[0], mac[1],
			mac[2], mac[3], mac[4], mac[5]);

	return String(result);
}

void bbaEspUploadManager::update_started() {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] CALLBACK:  HTTP update process started.\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::dbg_log(
				"[UPDATE MANAGER] CALLBACK:  HTTP update process started.");
	}
#endif
}

void bbaEspUploadManager::update_finished() {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] CALLBACK:  HTTP update process finished\n");
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::info_log(
				"[UPDATE MANAGER] CALLBACK:  HTTP update process finished");
		bbaEspUploadManager::count = 0;
	}
#endif
}

void bbaEspUploadManager::update_progress(const int &cur, const int &total) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] CALLBACK:  HTTP update process at %d of %d bytes...\n",
			cur, total);
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		if (bbaEspUploadManager::count == 0) {
			bbaServerLog::dbg_log(
					"[UPDATE MANAGER] CALLBACK:  HTTP update process at %d of %d bytes...",
					cur, total);
		}
		bbaEspUploadManager::count++;
	}
#endif
}

void bbaEspUploadManager::update_error(const int &err) {
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
	bbaSerial::dbg_printf(
			"[UPDATE MANAGER] CALLBACK:  HTTP update fatal error code %d\n",
			err);
#endif
#if ISDEFINED(SERVER_UPDATE_MANAGER_LOG)
	if (bbaLogSymbols::getUpdateManagerLog() && bbaWifiManager::isConnected()) {
		bbaServerLog::error_log(
				"[UPDATE MANAGER] CALLBACK:  HTTP update fatal error code %d",
				err);
		bbaEspUploadManager::count = 0;
	}
#endif
}

void bbaEspUploadManager::steamUpdate(Stream &streamVar,
		const uint16_t &firmwareLengthInBytes) {
	Update.begin(firmwareLengthInBytes);
	Update.writeStream(streamVar);
	Update.end();

	using THandlerFunction_Progress = std::function<void(size_t, size_t)>;
}
