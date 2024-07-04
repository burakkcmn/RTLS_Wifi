/*
 * spiEEPROM.h
 *
 *  Created on: Feb 22, 2022
 *      Author: aress
 */

#ifndef WIREDCOMMUNICATION_SPIEEPROM_H_
#define WIREDCOMMUNICATION_SPIEEPROM_H_

#include <Arduino.h>
#include <SPI.h>

/************EEPROM opcodes: commands to the 25LC1024 memory chip ******************/
#define WRITE 2
#define READ  3
#define WREN  6
#define WRDI 4
#define RDSR  5
#define WRSR  1
#define PE 66
#define SE  216
#define CE  199
#define RDID 171
#define DPD  185

extern uint8_t CS;		    // Global variable for CS pin (default 10)

class spiEEPROM {
public:
	spiEEPROM();
	virtual ~spiEEPROM();
	void SetMode(uint8_t CSpin);
	void WriteByte(uint32_t address, uint8_t data_uint8_t);
	uint8_t ReadByte(uint32_t address);
	void WriteByteArray(uint32_t address, uint8_t *data, uint16_t big);
	void ReadByteArray(uint32_t address, uint8_t *data, uint16_t big);
	void WriteInt(uint32_t address, int data);
	int ReadInt(uint32_t address);
	void WriteIntArray(uint32_t address, int *data, uint16_t big);
	void ReadIntArray(uint32_t address, int *data, uint16_t big);
	void WriteUnsignedInt(uint32_t address, unsigned int data);
	unsigned int ReadUnsignedInt(uint32_t address);
	void WriteUnsignedIntArray(uint32_t address, unsigned int *data,
			uint16_t big);
	void ReadUnsignedIntArray(uint32_t address, unsigned int *data,
			uint16_t big);
	void WriteLong(uint32_t address, long data);
	long ReadLong(uint32_t address);
	void WriteLongArray(uint32_t address, long *data, uint16_t big);
	void ReadLongArray(uint32_t address, long *data, uint16_t big);
	void WriteUnsignedLong(uint32_t address, unsigned long data);
	unsigned long ReadUnsignedLong(uint32_t address);
	void WriteUnsignedLongArray(uint32_t address, unsigned long *data,
			uint16_t big);
	void ReadUnsignedLongArray(uint32_t address, unsigned long *data,
			uint16_t big);
	void WriteFloat(uint32_t address, float data);
	float ReadFloat(uint32_t address);
	void WriteFloatArray(uint32_t address, float *data, uint16_t big);
	void ReadFloatArray(uint32_t address, float *data, uint16_t big);
};

#endif /* WIREDCOMMUNICATION_SPIEEPROM_H_ */

/* *********************************************************************************
 *
 *******************************************RWarrays*********************************
 #define CSPIN 10       // Default Chip Select Line for Uno (change as needed)

 spiEEPROM EEPROM;       // initialize an instance of this class

 /*******************  Create some dummy data to read and space to write  ******************
 byte a[20] = { "abcdefghijklmnopqrs" };         // array data to write
 int b[5] = { 256, 257, 258, 259, 260 };             // array data to write
 unsigned int c[5] = { 32768, 32769, 32770, 32771, 32772 }; // array data to write
 long d[5] = { 65536, 65537, 65538, 65539, 65540 };
 unsigned long e[5] = { 2147483648, 2147483649, 2147483650, 2147483651,
 2147483652 };
 float f[5] = { 3.14, 1.59, 2.67, 8.45, 9.99 };

 byte read_data_a[20];                     // array to hold data read from memory
 int read_data_b[5];                       // array to hold data read from memory
 unsigned int read_data_c[5];              // array to hold data read from memory
 long read_data_d[5];                      // array to hold data read from memory
 unsigned long read_data_e[5];             // array to hold data read from memory
 float read_data_f[5];                     // array to hold data read from memory

 void setup() {
 uint32_t address = 0; // create a 32 bit variable to hold the address (uint32_t=long)
 byte value;                   // create variable to hold the data value read
 byte data;                    // create variable to hold the data value sent
 Serial.begin(9600);        // set communication speed for the serial monitor
 SPI.begin();                     // start communicating with the memory chip

 /************  Write a Sequence of Bytes from an Array *******************
 Serial.println("\nWriting byte array using Sequential: ");
 EEPROM.WriteByteArray(0, a, sizeof(a)); // Write array e to memory starting at address 0

 /************ Read a Sequence of Bytes from Memory into an Array **********
 Serial.println("Reading byte array using sequential: ");
 EEPROM.ReadByteArray(0, read_data_a, sizeof(read_data_a)); // Read array into read_data_a starting at address 0
 for (int i = 0; i < sizeof(read_data_a); i++) {     // print the array
 Serial.println((char) read_data_a[i]);   // We need to cast it as a char
 }                                         // to make it print as a character

 /************  Write an Integer Array *******************
 Serial.println("\nWriting integer array using Sequential: ");
 EEPROM.WriteIntArray(0, b, 5); // Write array b to memory starting at address 0

 /************ Read an Integer Array **********
 Serial.println("Reading integer array using sequential: ");
 EEPROM.ReadIntArray(0, read_data_b, 5); // Read from memory into empty array read_data_b
 for (int i = 0; i < 5; i++) {          // Output read data to serial monitor
 Serial.println(read_data_b[i]);
 }

 /************  Write an Unsigned Integer Array *******************
 Serial.println("\nWriting unsigned integer array using Sequential: ");
 EEPROM.WriteUnsignedIntArray(0, c, 5); // Use the array of characters defined in z above
 // write to memory starting at address 0
 /************ Read an Unsigned Integer Array **********
 Serial.println("Reading unsigned integer array using sequential: ");
 EEPROM.ReadUnsignedIntArray(0, read_data_c, 5); // Read from memory into empty array read_data_c
 for (int i = 0; i < 5; i++) {          // Output read data to serial monitor
 Serial.println(read_data_c[i]);
 }

 /************  Write a Long Array *******************
 Serial.println("\nWriting long array: ");
 EEPROM.WriteLongArray(0, d, 5); // Write array d to memory starting at address 0

 /************ Read a Long Array **********
 Serial.println("Reading long array: ");
 EEPROM.ReadLongArray(0, read_data_d, 5); // Read from memory into empty array read_data_d
 for (int i = 0; i < 5; i++) {          // Output read data to serial monitor
 Serial.println(read_data_d[i]);
 }

 /************  Write an Unsigned Long Array *******************
 Serial.println("\nWriting unsigned long array: ");
 EEPROM.WriteUnsignedLongArray(0, e, 5); // Write array e to memory starting at address 0

 /************ Read an Unsigned Long Array **********
 Serial.println("Reading unsigned long array: ");
 EEPROM.ReadUnsignedLongArray(0, read_data_e, 5); // Read from memory into empty array read_data_e
 for (int i = 0; i < 5; i++) {          // Output read data to serial monitor
 Serial.println(read_data_e[i]);
 }

 /************  Write a Float Array *******************
 Serial.println("\nWriting float array: ");
 EEPROM.WriteFloatArray(0, f, 5); // Write array f to memory starting at address 0

 /************ Read Float Array **********
 Serial.println("Reading float array: ");
 EEPROM.ReadFloatArray(0, read_data_f, 5); // Read from memory into empty array read_data_f
 for (int i = 0; i < 5; i++) {          // Output read data to serial monitor
 Serial.println(read_data_f[i]);
 }
 }

 void loop() {
 }
 *****************************************RWSingle************************************
 #define CSPIN 10       // Default Chip Select Line for Uno (change as needed)
 spiEEPROM EEPROM;       //initialize an instance of this class

 /*******  Set up code to define variables and start the SCI and SPI serial interfaces  *****
 void setup()
 {
 uint32_t address = 0;                       // create a 32 bit variable to hold the address (uint32_t=long)
 Serial.begin(9600);                         // set communication speed for the serial monitor
 SPI.begin();                                // start communicating with the memory chip

 // And now the fun begins:
 /**********Write a Single Byte *******************
 byte data = 0;                              // initialize the data
 for(int i = 0; i <=5; i++){                 // Let's write 5 individual bytes to memory
 address = i;                              // use the loop counter as the address
 EEPROM.WriteByte(address, data);            // now write the data to that address
 data+=2;                                  // increment the data by 2
 }

 /********* Read a single Byte *********************
 Serial.println("Reading each data byte individually: ");
 byte value;                                 // create variable to hold the data value read
 for(int i = 0; i <=5; i++){                 // start at memory location 0 and end at 5
 address = i;                              // use the loop counter as the memory address
 value = EEPROM.ReadByte(address);           // reads a byte of data at that memory location
 Serial.println(value);                    // Let's see what we got
 }

 /************  Write an Integer *******************
 Serial.println("\nWriting integer using sequential: ");
 int tempInt1=-32768;                         // highest integer # is 32767
 EEPROM.WriteInt(0, tempInt1);                  // send tempInt1 to EEPROM starting from address 0

 /************ Read an Integer from Memory into an Int **********
 Serial.println("Reading integer using sequential: ");
 int tempInt2=EEPROM.ReadInt(0);                // Read integer from memory address 0
 Serial.println(tempInt2);                    // print as integer

 /************  Write an Unsigned Integer *******************
 Serial.println("\nWriting unsigned integer using sequential: ");
 unsigned int tempUnsignedInt1=65535;         // highest unsigned integer is 65535
 EEPROM.WriteInt(0, tempUnsignedInt1);          // send tempUnsignedInt1 to EEPROM starting from address 0

 /************ Read an Unsigned Integer from Memory into an Int **********
 Serial.println("Reading unsigned integer using sequential: ");
 unsigned int tempUnsignedInt2=EEPROM.ReadUnsignedInt(0);     // Read integer from memory address 0
 Serial.println(tempUnsignedInt2);            // print as unsigned integer

 /************  Write Long *******************
 Serial.println("\nWriting long using sequential: ");
 long tempLong1=2147483647;                   // highest long #
 EEPROM.WriteLong(0, tempLong1);                // send tempLong1 to EEPROM starting from address 0

 /************ Read Long from Memory **********
 Serial.println("Reading long using sequential: ");
 long tempLong2=EEPROM.ReadLong(0);             // Read long from memory address 0
 Serial.println(tempLong2);                   // print as long

 /************  Write Unsigned Long *******************
 Serial.println("\nWriting unsigned long using sequential: ");
 long tempUnsignedLong1=4294967295;           // highest unsigned long #
 EEPROM.WriteUnsignedLong(0, tempUnsignedLong1);        // send tempUnsignedLong1 to EEPROM starting from address 0

 /************ Read Unsigned Long from Memory **********
 Serial.println("Reading unsigned long using sequential: ");
 unsigned long tempUnsignedLong2=EEPROM.ReadUnsignedLong(0);  // Read unsigned long from memory address 0
 Serial.println(tempUnsignedLong2);                   // print as unsigned long

 /************  Write a Float using Sequential *******************
 Serial.println("\nWriting float using sequential: ");
 float tempFloat1=3.141593;                   // a float #
 EEPROM.WriteFloat(0, tempFloat1);              // send tempFloat1 to EEPROM starting at address 0

 /************ Read Float from Memory into a Float **********
 Serial.println("Reading float using sequential: ");
 float tempFloat2=EEPROM.ReadFloat(0);           // Read float from memory address 0
 Serial.println(tempFloat2,6);                 // print as float
 }

 void loop(){
 }
 *
 *****************************************RWStruct**********************************
 #define CSPIN 10       // Default Chip Select Line for Uno (change as needed)

 spiEEPROM EEPROM;       // initialize an instance of this class

 void setup()
 {
 Serial.begin(9600);                         // set communication speed for the serial monitor
 SPI.begin();                                // start communicating with the memory chip

 /*******************  Create a dummy structure and objects  ******************
 struct chemElements {  // declaring a structured array
 char atomName[10];   // size of largest length char array + 1 for null character
 int atomNum;
 float MW;
 }; //declare organicA to send data and organicB to receive data

 union txUnion{
 chemElements organicA[4]; // object organicA as an array of chemElements
 byte byteArrA[sizeof(organicA)]; // byte array sharing the same space
 };
 union txUnion txData; // create a new union instance called txData

 // Fill objects with data
 strcpy(txData.organicA[0].atomName,"Hydrogen");
 txData.organicA[0].atomNum=1;
 txData.organicA[0].MW=1.000794;

 strcpy(txData.organicA[1].atomName,"Carbon");
 txData.organicA[1].atomNum=6;
 txData.organicA[1].MW=12.0107;

 strcpy(txData.organicA[2].atomName,"Nitrogen");
 txData.organicA[2].atomNum=7;
 txData.organicA[2].MW=14.0067;

 strcpy(txData.organicA[3].atomName,"Oxygen");
 txData.organicA[3].atomNum=8;
 txData.organicA[3].MW=15.9994;

 /************  Write a Sequence of Bytes from an Array *******************
 Serial.println("\nWriting byte array using Sequential:");
 EEPROM.WriteByteArray(0, txData.byteArrA, sizeof(txData.byteArrA));        // Write array to memory starting at address 0

 /************ Read a Sequence of Bytes from Memory into an Array **********
 union rxUnion{  // create a union for receiving data and reading as chemElements structure
 chemElements organicB[4];  // object organicB as an array of chemElements
 byte byteArrB[sizeof(organicB)]; // byte array sharing the same space
 };
 union rxUnion rxData; // create a new union instance called rxData

 Serial.println("Reading byte array using sequential: ");
 EEPROM.ReadByteArray(0, rxData.byteArrB, sizeof(rxData.byteArrB));   // Read array into byteArrB starting at address 0

 for(int i=0;i<4;i++){
 Serial.println(rxData.organicB[i].atomName); // print received char array
 Serial.println(rxData.organicB[i].atomNum); // print received int
 Serial.println(rxData.organicB[i].MW,4); // print received float
 }
 }

 void loop()
 {
 }
 * ****************************************************************************** */

