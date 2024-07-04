/*
 * bbaLogSymbols.h
 *
 *  Created on: Feb 18, 2023
 *      Author: aress
 */

#ifndef UTILS_SYMBOLS_BBALOGSYMBOLS_H_
#define UTILS_SYMBOLS_BBALOGSYMBOLS_H_

#define ISDEFINED(module) \
    ((defined(module ## _ENABLED) && (module ## _ENABLED)) ? 1 : 0)

/**************************SERVER LOGS*******************************/
#ifndef SERVER_GPIO_LOG_ENABLED
#define SERVER_GPIO_LOG_ENABLED 0
#endif

#ifndef SERVER_EEPROM_LOG_ENABLED
#define SERVER_EEPROM_LOG_ENABLED 0
#endif

#ifndef SERVER_WIFI_LOG_ENABLED
#define SERVER_WIFI_LOG_ENABLED 1
#endif

#ifndef SERVER_UPDATE_MANAGER_LOG_ENABLED
#define SERVER_UPDATE_MANAGER_LOG_ENABLED 1
#endif

#ifndef SERVER_WEB_SERVICE_LOG_ENABLED
#define SERVER_WEB_SERVICE_LOG_ENABLED 1
#endif

#ifndef SERVER_HEARTBEAT_LOG_ENABLED
#define SERVER_HEARTBEAT_LOG_ENABLED 1
#endif

#ifndef SERVER_MESSAGEBUFFER_LOG_ENABLED
#define SERVER_MESSAGEBUFFER_LOG_ENABLED 1
#endif

#ifndef SERVER_SERIAL_HANDLER_LOG_ENABLED
#define SERVER_SERIAL_HANDLER_LOG_ENABLED 1
#endif

#ifndef SERVER_MAIN_LOG_ENABLED
#define SERVER_MAIN_LOG_ENABLED 1
#endif

#ifndef SERVER_STRING_PROCESS_LOG_ENABLED
#define SERVER_STRING_PROCESS_LOG_ENABLED 1
#endif

#ifndef SERVER_SERVER_COMMANDS_HANDLER_LOG_ENABLED
#define SERVER_SERVER_COMMANDS_HANDLER_LOG_ENABLED 1
#endif

#ifndef SERVER_NRF52XX_BOOTLOADER_LOG_ENABLED
#define SERVER_NRF52XX_BOOTLOADER_LOG_ENABLED 1
#endif

/**************************SERIAL LOGS*******************************/

#ifndef SERIAL_GPIO_LOG_ENABLED
#define SERIAL_GPIO_LOG_ENABLED 1
#endif

#ifndef SERIAL_EEPROM_LOG_ENABLED
#define SERIAL_EEPROM_LOG_ENABLED 1
#endif

#ifndef SERIAL_WIFI_LOG_ENABLED
#define SERIAL_WIFI_LOG_ENABLED 1
#endif

#ifndef SERIAL_UPDATE_MANAGER_LOG_ENABLED
#define SERIAL_UPDATE_MANAGER_LOG_ENABLED 1
#endif

#ifndef SERIAL_WEB_SERVICE_LOG_ENABLED
#define SERIAL_WEB_SERVICE_LOG_ENABLED 1
#endif

#ifndef SERIAL_HEARTBEAT_LOG_ENABLED
#define SERIAL_HEARTBEAT_LOG_ENABLED 1
#endif

#ifndef SERIAL_MESSAGEBUFFER_LOG_ENABLED
#define SERIAL_MESSAGEBUFFER_LOG_ENABLED 1
#endif

#ifndef SERIAL_SERIAL_HANDLER_LOG_ENABLED
#define SERIAL_SERIAL_HANDLER_LOG_ENABLED 1
#endif

#ifndef SERIAL_LOCATION_PACKET_LOG_ENABLED
#define SERIAL_LOCATION_PACKET_LOG_ENABLED 1
#endif

#ifndef SERIAL_LOG_PACKET_LOG_ENABLED
#define SERIAL_LOG_PACKET_LOG_ENABLED 1
#endif

#ifndef SERIAL_REGISTRATION_PACKET_LOG_ENABLED
#define SERIAL_REGISTRATION_PACKET_LOG_ENABLED 1
#endif

#ifndef SERIAL_MAIN_LOG_ENABLED
#define SERIAL_MAIN_LOG_ENABLED 1
#endif

#ifndef SERIAL_STRING_PROCESS_LOG_ENABLED
#define SERIAL_STRING_PROCESS_LOG_ENABLED 1
#endif

#ifndef SERIAL_UDP_CLIENT_MANAGER_LOG_ENABLED
#define SERIAL_UDP_CLIENT_MANAGER_LOG_ENABLED 1
#endif

#ifndef SERIAL_SERVER_COMMANDS_HANDLER_LOG_ENABLED
#define SERIAL_SERVER_COMMANDS_HANDLER_LOG_ENABLED 1
#endif

#ifndef SERIAL_NRF52XX_BOOTLOADER_LOG_ENABLED
#define SERIAL_NRF52XX_BOOTLOADER_LOG_ENABLED 1
#endif

class bbaLogSymbols final {
private:

	static bool gpioControllerLog;
	static bool eepromLog;
	static bool wifiLog;
	static bool updateManagerLog;
	static bool webServiceLog;
	static bool heartbeatLog;
	static bool messageBufferLog;
	static bool locationPacketLog;
	static bool logPacketLog;
	static bool registrationPacketLog;
	static bool serialHandlerLog;
	static bool mainLog;
	static bool stringProcessLog;
	static bool serverCommandHandlerLog;
	static bool nrf52xxBootloaderLog;

public:

	static bool getGpioControllerLog();
	static bool getEepromLog();
	static bool getWifiLog();
	static bool getUpdateManagerLog();
	static bool getWebServiceLog();
	static bool getHeartbeatLog();
	static bool getMessageBufferLog();
	static bool getLocationPacketLog();
	static bool getLogPacketLog();
	static bool getSerialHandlerLog();
	static bool getRegistrationPacketLog();
	static bool getMainLog();
	static bool getStringProcessLog();
	static bool getServerCommandHandlerLog();
	static bool getNrf52xxBootloaderLog();

};

#endif /* UTILS_SYMBOLS_BBALOGSYMBOLS_H_ */
