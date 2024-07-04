/*
 * createServer.h
 *
 *  Created on: Dec 29, 2022
 *      Author: aress
 */

#ifndef WEB_WEBSERVER_H_
#define WEB_WEBSERVER_H_

#include <ESP8266WebServer.h>

class webServer final {
public:
	webServer();
	virtual ~webServer();
	void begin(uint16_t port);
	void handle();
private:
	uint16_t port;
	String header;
	ESP8266WebServer server;

	void handleTEST();
	void handleNOTFOUND();
	void handleStPORTAL();
	void handleApPORTAL();
	bool updateSTAInfo(std::string ssid, std::string password, std::string channel, std::string hidden, std::string maxcount);
	bool updateApInfo(std::string ssid, std::string password, std::string channel, std::string hidden, std::string maxcount);
	void restart();
};

#endif /* WEB_WEBSERVER_H_ */
