/*
 * bbaEspUploadManager.h
 *
 *  Created on: Jan 17, 2022
 *      Author: aress
 */

#ifndef BBAUPDATEMANAGER_H_
#define BBAUPDATEMANAGER_H_

#include "../utils/symbols/bbaGlobalSymbols.h"
#if ISDEFINED(SERIAL_UPDATE_MANAGER_LOG)
#define DEBUG_OTA_UPDATE true
#endif

#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266mDNS.h>

class bbaEspUploadManager { public: enum UpdateMode { WEB = 0, IDE
,	HTTP
};

void OtaConfig(const int&);
void otaHandle();
void checkForUpdates(const String&, const String&);
String getMAC();
void httpUpdateNow();
void httpsUpdateNow();
void steamUpdate(Stream &buffer, const uint16_t &length);

private:
/**************************WEB SETTINGS**************************/
/**************************IDE SETTINGS**************************/
String otaIdeHostName = "ESP8266-OTA-";
String otaIdePass = "321";
uint16_t otaIdePort = 8266;

bool WebOta = false;
bool IdeOta = false;

UpdateMode updateMode;

ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer httpServer;

static uint8_t count;

static void update_started();
static void update_finished();
static void update_progress(const int &cur, const int &total);
static void update_error(const int &err);
};

#endif /* BBAUPDATEMANAGER_H_ */
