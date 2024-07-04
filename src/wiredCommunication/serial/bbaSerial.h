/*
 * bbaSerial.h
 *
 *  Created on: Jan 21, 2022
 *      Author: aress
 */

#ifndef BBASERIAL_H_
#define BBASERIAL_H_

#include <SoftwareSerial.h>
#include <string>
#include <stdint.h>
#include <stdarg.h>

using namespace std;

class bbaSerial {
public:
	bbaSerial();
	virtual ~bbaSerial();
	void begin(uint32_t baudrate, int8_t rxPin, int8_t txPin);
	void printf(const char *format, ...);
	static void setup(int);
	static void dbg_printf(const char *format, ...);
	static void dbg_printBits(uint8_t);
	static std::string read();

private:
//	SoftwareSerial serialPort;
//	int prevBaudrate;
};

#endif /* BBASERIAL_H_ */
