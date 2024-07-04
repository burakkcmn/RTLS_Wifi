/*
 * bbaSerial.cpp
 *
 *  Created on: Jan 21, 2022
 *      Author: aress
 */

#include "bbaSerial.h"
#include "../../utils/symbols/bbaGlobalSymbols.h"

bbaSerial::bbaSerial() {
//	prevBaudrate = Serial.baudRate();
//	Serial.end();
}

bbaSerial::~bbaSerial() {
//	Serial.begin(prevBaudrate);
}

void bbaSerial::begin(uint32_t baudrate, int8_t rxPin, int8_t txPin) {
//	serialPort.begin(baudrate, SoftwareSerialConfig::SWSERIAL_5N1, rxPin, txPin);
}

void bbaSerial::printf(const char *format, ...) {
	static char sbuf[1400];
	va_list varArgs;

	va_start(varArgs, format);
	vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
	va_end(varArgs);

//	serialPort.print(sbuf);
}

void bbaSerial::setup(int baudrate) {
	Serial.begin(baudrate);
	Serial.pins(bbaGlobalSymbols::getEspTxPin(),
			bbaGlobalSymbols::getEspRxPin());
}

void bbaSerial::dbg_printf(const char *format, ...) {
	static char sbuf[1400];
	va_list varArgs;

	va_start(varArgs, format);
	vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
	va_end(varArgs);

	Serial.print(sbuf);
}

void bbaSerial::dbg_printBits(uint8_t num) {
	uint8_t size = sizeof(uint8_t);
	uint8_t maxPow = 1 << (size * 8 - 1);
	//printf("MAX POW : %u\n",maxPow);
	uint8_t i = 0, j;
	for (; i < size; ++i) {
		for (j = 0; j < 8; ++j) {
			// print last bit and shift left.
			Serial.print(num & maxPow ? 1 : 0);
			num = num << 1;
		}
	}
}

std::string bbaSerial::read() {
	std::string data = "";
	int count = Serial.available();
	if (count > 0) {
		char cval[bbaGlobalSymbols::getSerialMaxBufferSize()];
		count = min(count, bbaGlobalSymbols::getSerialMaxBufferSize() - 1);
		for (int i = 0; i < count; ++i) {
			char ch = Serial.read();
			cval[i] = ch;
		}
		cval[count] = '\0';
		data = cval;

	}
	return data;
}
