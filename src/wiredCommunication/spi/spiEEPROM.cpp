/*
 * spiEEPROM.cpp
 *
 *  Created on: Feb 22, 2022
 *      Author: aress
 */

#include "spiEEPROM.h"

#include "../../utils/symbols/bbaGlobalSymbols.h"
#if ISDEFINED(SERIAL_EEPROM_LOG)
#include "../serial/bbaSerial.h"
#endif

spiEEPROM::spiEEPROM() {
}

spiEEPROM::~spiEEPROM() {
}

uint8_t CS = 15; // default CS global variable
//#define WRITEDELAY 10   // delay for writing to chip

/*************  Set up CSpin and SPI settings *******************/
void spiEEPROM::SetMode(uint8_t CSpin) {
	CS = CSpin; // set global variable CS to user-defined CS pin
	pinMode(CS, OUTPUT);	                        // set CS pin to output mode
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV2); // max clock is 20MHz, so can set high speed
}

/************ Byte transfer functions ***************************/
void spiEEPROM::WriteByte(uint32_t address, uint8_t data) {
	SetMode(CS);                 // set to send/receive single byte of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(data);
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is important!
}

uint8_t spiEEPROM::ReadByte(uint32_t address) {
	SetMode(CS);
	char data;
	digitalWrite(CS, LOW);
	SPI.transfer(READ);
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	data = SPI.transfer(0x00);
	digitalWrite(CS, HIGH);
	return data;
}

/*********** Sequential data transfer functions using Arrays ************************/
void spiEEPROM::WriteByteArray(uint32_t address, uint8_t *data, uint16_t big) {
	SetMode(CS);                // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(data, big);             // write the data bytes
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is needed
}

void spiEEPROM::ReadByteArray(uint32_t address, uint8_t *data, uint16_t big) {
	SetMode(CS);                   // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(READ); // read instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < big; i++) {
		data[i] = SPI.transfer(0x00);     // read the data bytes
	}
	digitalWrite(CS, HIGH);
}

/*********** Write and Read an Integer (2 bytes) ************************/
void spiEEPROM::WriteInt(uint32_t address, int data) {
	uint8_t temp[2]; 				 // temporary array of bytes with 2 elements
	temp[0] = (uint8_t) (data >> 8);                     // high byte of integer
	temp[1] = (uint8_t) (data);                           // low byte of integer
	SetMode(CS);                 // set to send/receive single byte of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, 2); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is important!
}

int spiEEPROM::ReadInt(uint32_t address) {
	uint8_t temp[2]; 				 // temporary array of bytes with 2 elements
	int data = 0;
	SetMode(CS);
	digitalWrite(CS, LOW);
	SPI.transfer(READ);
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < 2; i++) {
		temp[i] = SPI.transfer(0x00);                 // read the two bytes
	}
	digitalWrite(CS, HIGH);                         // set SPI slave select HIGH
	data = ((int) temp[0] << 8) + temp[1];          // data=high byte & low byte
	return data;
}

/*********** Write and Read an Integer Array ************************/
void spiEEPROM::WriteIntArray(uint32_t address, int *data, uint16_t big) {
	uint8_t temp[big * 2]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;                                   // byte counter
	for (uint16_t i = 0; i < big; i++) { // take apart integers into high and low bytes
		temp[j] = (uint8_t) (data[i] >> 8);              // high byte of integer
		temp[j + 1] = (uint8_t) (data[i]);                // low byte of integer
		j += 2;                                         // increment counter
	}
	SetMode(CS);                // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, big * 2); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is needed
}

void spiEEPROM::ReadIntArray(uint32_t address, int *data, uint16_t big) {
	uint8_t temp[big * 2]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;                                   // byte counter
	SetMode(CS);                   // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(READ); // read instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < (big * 2); i++) {
		temp[i] = SPI.transfer(0x00);                 // read the data byte
	}
	digitalWrite(CS, HIGH);
	for (uint16_t i = 0; i < big; i++) {
		data[i] = ((int) temp[j] << 8) + temp[j + 1]; // data=high byte & low byte
		j += 2;                                         // increment counter
	}
}

/*********** Write and Read an Unsigned Integer (2 bytes) ************************/
void spiEEPROM::WriteUnsignedInt(uint32_t address, unsigned int data) {
	uint8_t temp[2]; 				 // temporary array of bytes with 2 elements
	temp[0] = (uint8_t) (data >> 8);                     // high byte of integer
	temp[1] = (uint8_t) (data);                           // low byte of integer
	SetMode(CS);                 // set to send/receive single byte of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, 2); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is important!
}

unsigned int spiEEPROM::ReadUnsignedInt(uint32_t address) {
	uint8_t temp[2]; 				 // temporary array of bytes with 2 elements
	unsigned int data = 0;
	SetMode(CS);
	digitalWrite(CS, LOW);
	SPI.transfer(READ);
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < 2; i++) {
		temp[i] = SPI.transfer(0x00);                 // read the two bytes
	}
	digitalWrite(CS, HIGH);                         // set SPI slave select HIGH
	data = ((int) temp[0] << 8) + temp[1];          // data=high byte & low byte
	return data;
}

/*********** Write and Read an Unsigned Integer Array ************************/
void spiEEPROM::WriteUnsignedIntArray(uint32_t address, unsigned int *data, uint16_t big) {
	uint8_t temp[big * 2]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;                                   // byte counter
	for (uint16_t i = 0; i < big; i++) { // take apart integers into high and low bytes
		temp[j] = (uint8_t) (data[i] >> 8);              // high byte of integer
		temp[j + 1] = (uint8_t) (data[i]);                // low byte of integer
		j += 2;                                         // increment counter
	}
	SetMode(CS);                // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, big * 2); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is needed
}

void spiEEPROM::ReadUnsignedIntArray(uint32_t address, unsigned int *data, uint16_t big) {
	uint8_t temp[big * 2]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;                                   // byte counter
	SetMode(CS);                   // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(READ); // read instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < (big * 2); i++) {
		temp[i] = SPI.transfer(0x00);                 // read the data byte
	}
	digitalWrite(CS, HIGH);
	for (uint16_t i = 0; i < big; i++) {
		data[i] = ((int) temp[j] << 8) + temp[j + 1]; // data=high byte & low byte
		j += 2;                                         // increment counter
	}
}

/*********** Write and Read a Long (4 bytes) ************************/
void spiEEPROM::WriteLong(uint32_t address, long data) {
	uint8_t temp[4]; 				 // temporary array of bytes with 4 elements
	temp[0] = (uint8_t) (data >> 24);                    // high byte of integer
	temp[1] = (uint8_t) (data >> 16);                     // low byte of integer
	temp[2] = (uint8_t) (data >> 8);                      // low byte of integer
	temp[3] = (uint8_t) (data);                           // low byte of integer
	SetMode(CS);                 // set to send/receive single byte of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, 4); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is important!
}

long spiEEPROM::ReadLong(uint32_t address) {
	uint8_t temp[4]; 				 // temporary array of bytes with 4 elements
	long data = 0;
	SetMode(CS);
	digitalWrite(CS, LOW);
	SPI.transfer(READ);
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < 4; i++) {
		temp[i] = SPI.transfer(0x00);                 // read the data byte
	}
	digitalWrite(CS, HIGH);                         // set SPI slave select HIGH
	data = ((long) temp[0] << 24) + ((long) temp[1] << 16) + ((long) temp[2] << 8) + temp[3];   // reassemble bytes into long
	return data;
}

/*********** Write and Read a Long Array ************************/
void spiEEPROM::WriteLongArray(uint32_t address, long *data, uint16_t big) {
	uint8_t temp[big * 4]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;                                   // byte counter
	for (uint16_t i = 0; i < big; i++) { // take apart integers into high and low bytes
		temp[j] = (uint8_t) (data[i] >> 24);             // high byte of integer
		temp[j + 1] = (uint8_t) (data[i] >> 16);          // low byte of integer
		temp[j + 2] = (uint8_t) (data[i] >> 8);           // low byte of integer
		temp[j + 3] = (uint8_t) data[i];                  // low byte of integer
		j += 4;                                         // increment counter
	}
	SetMode(CS);                // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, big * 4); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is needed
}

void spiEEPROM::ReadLongArray(uint32_t address, long *data, uint16_t big) {
	uint8_t temp[big * 4]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;                                   // byte counter
	SetMode(CS);                   // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(READ); // read instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < (big * 4); i++) {
		temp[i] = SPI.transfer(0x00);                 // read the data byte
	}
	digitalWrite(CS, HIGH);
	for (uint16_t i = 0; i < big; i++) {
		data[i] = ((long) temp[j] << 24) + ((long) temp[j + 1] << 16) + ((long) temp[j + 2] << 8) + temp[j + 3]; // put together 4 bytes
		j += 4;                                         // increment counter
	}
}

/*********** Write and Read an Unsigned Long (4 bytes) ************************/
void spiEEPROM::WriteUnsignedLong(uint32_t address, unsigned long data) {
	uint8_t temp[4]; 			 	 // temporary array of bytes with 4 elements
	temp[0] = (uint8_t) (data >> 24);                    // high byte of integer
	temp[1] = (uint8_t) (data >> 16);                     // low byte of integer
	temp[2] = (uint8_t) (data >> 8);                      // low byte of integer
	temp[3] = (uint8_t) (data);                           // low byte of integer
	SetMode(CS);                // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, 4); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is needed
}

unsigned long spiEEPROM::ReadUnsignedLong(uint32_t address) {
	uint8_t temp[4]; 				 // temporary array of bytes with 4 elements
	unsigned long data = 0;
	SetMode(CS);
	digitalWrite(CS, LOW);
	SPI.transfer(READ);
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < 4; i++) {
		temp[i] = SPI.transfer(0x00);                 // read the data byte
	}
	digitalWrite(CS, HIGH);                         // set SPI slave select HIGH
	data = ((long) temp[0] << 24) + ((long) temp[1] << 16) + ((long) temp[2] << 8) + temp[3];   // reassemble bytes into long
	return data;
}

/*********** Write and Read a Long Array ************************/
void spiEEPROM::WriteUnsignedLongArray(uint32_t address, unsigned long *data, uint16_t big) {
	uint8_t temp[big * 4]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;                                   // byte counter
	for (uint16_t i = 0; i < big; i++) { // take apart integers into high and low bytes
		temp[j] = (uint8_t) (data[i] >> 24);             // high byte of integer
		temp[j + 1] = (uint8_t) (data[i] >> 16);          // low byte of integer
		temp[j + 2] = (uint8_t) (data[i] >> 8);           // low byte of integer
		temp[j + 3] = (uint8_t) data[i];                  // low byte of integer
		j += 4;                                         // increment counter
	}
	SetMode(CS);                // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, big * 4); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is needed	                       // set SPI slave select HIGH
}

void spiEEPROM::ReadUnsignedLongArray(uint32_t address, unsigned long *data, uint16_t big) {
	uint8_t temp[big * 4]; 			// temporary array of bytes with 2 elements
	uint16_t j = 0;
	SetMode(CS);                   // set to send/receive multiple bytes of data
	digitalWrite(CS, LOW);
	SPI.transfer(READ); // read instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < (big * 4); i++) {
		temp[i] = SPI.transfer(0x00);                 // read the data byte
	}
	digitalWrite(CS, HIGH);
	for (uint16_t i = 0; i < big; i++) {
		data[i] = ((long) temp[j] << 24) + ((long) temp[j + 1] << 16) + ((long) temp[j + 2] << 8) + temp[j + 3]; // put together 4 bytes
		j += 4;                                         // increment counter
	}
}

/*********** Write and Read a Float (4 bytes) ************************/
void spiEEPROM::WriteFloat(uint32_t address, float data) {
	uint8_t *temp = (uint8_t*) &data;                // split float into 4 bytes
	SetMode(CS);                 // set to send/receive single byte of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(temp, 4); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is important!
}

float spiEEPROM::ReadFloat(uint32_t address) {
	uint8_t temp[4]; 				 // temporary array of bytes with 2 elements
	float data = 0;
	SetMode(CS);
	digitalWrite(CS, LOW);
	SPI.transfer(READ);
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < 4; i++) {
		temp[i] = SPI.transfer(0x00);                 // read the two bytes
	}
	digitalWrite(CS, HIGH);                         // set SPI slave select HIGH
	data = *(float*) &temp;                         // 4 bytes to a float.
	return data;	            // https://www.microchip.com/forums/m590535.aspx
}

/*********** Write and Read a Float Array ************************/
void spiEEPROM::WriteFloatArray(uint32_t address, float *data, uint16_t big) {
	uint8_t holder[big * 4]; 		// temporary array of bytes with 2 elements
	uint16_t j = 0;
	for (int i = 0; i < big; i++) {
		uint8_t *temp = (uint8_t*) &data[i];         // split float into 4 bytes
		holder[j] = temp[0];
		holder[j + 1] = temp[1];
		holder[j + 2] = temp[2];
		holder[j + 3] = temp[3];
		j += 4;
	}
	SetMode(CS);                 // set to send/receive single byte of data
	digitalWrite(CS, LOW);
	SPI.transfer(WREN);
	digitalWrite(CS, HIGH);
	digitalWrite(CS, LOW);
	SPI.transfer(WRITE); // write instruction
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	SPI.transfer(holder, big * 4); // transfer an array of data => needs array name & size (2 elements)
	digitalWrite(CS, HIGH);
//	delay (WRITEDELAY); // this delay is important!
}

void spiEEPROM::ReadFloatArray(uint32_t address, float *data, uint16_t big) {
	uint8_t holder[big * 4]; 				  // temporary array of bytes
	uint8_t t[4];                                      // to hold 4 bytes
	SetMode(CS);
	digitalWrite(CS, LOW);
	SPI.transfer(READ);
	SPI.transfer((uint8_t) (address >> 16));
	SPI.transfer((uint8_t) (address >> 8));
	SPI.transfer((uint8_t) address);
	for (uint16_t i = 0; i < (big * 4); i++) {
		holder[i] = SPI.transfer(0x00);               // read the data byte
	}
	digitalWrite(CS, HIGH);                         // set SPI slave select HIGH
	uint16_t j = 0;
	for (int i = 0; i < big; i++) {
		t[0] = holder[j];
		t[1] = holder[j + 1];
		t[2] = holder[j + 2];
		t[3] = holder[j + 3];
		data[i] = *(float*) &t;                        // 4 bytes to a float.
		j += 4;
	}
}
