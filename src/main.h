#ifndef RTLSESP8266_H_
#define RTLSESP8266_H_

#include "wirelessCommunication/bbaWifiManager.h"
#include "ServerCommunications/bbaHeartbeat.h"
#include "web/webServer.h"
#include "wiredCommunication/serial/bbaSerialHandler.h"
#include "wiredCommunication/serial/dwmCommunications/bbaDWMMessenger.h"
#include "utils/eepromUtil.h"
#include "utils/symbols/bbaGlobalSymbols.h"
//#include "bbaNrfxBootloaderManager.h"
#include "utils/bbaESPInformations.h"
#include "wiredCommunication/serial/bbaSerial.h"
#include "ServerCommunications/bbaServerCommandHandler.h"
#include "wirelessCommunication/bbaEspUpdateManager.h"
#include "wiredCommunication/bootloader/bbaNrf52xxBootloader.h"
#include "utils/StringProcesses.h"
#include "utils/bbaTasksUtils.h"
#include "wiredCommunication/gpio/bbaDwmGpioController.h"

#if ISDEFINED(SERVER_MAIN_LOG)
#include "utils/bbaServerLog.h"
#endif

bbaEspUploadManager espUploadManager;
bbaNrf52xxBootloader dwmUploadManager;
bbaHeartbeat heartbeat;
bbaSerialHandler serialHandler;
webServer webServer;
bbaDwmGpioController dwmGpiocontroller;
bbaServerCommandHandler serverCommandHadler;
bbaTasksUtils tasks;

#endif /* RTLSESP8266_H_ */
