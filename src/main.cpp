#include "main.h"

#ifndef UNITTEST

//The setup function is called once at startup of the sketch
void setup() {
	bbaSerial::setup(bbaGlobalSymbols::getSerialBaudrrate());

	bbaDWMMessenger::getLastErrorMessageRequest();

	eepromUtil::begin();

	bbaWifiManager::WifiLoad(true);

	espUploadManager.OtaConfig(bbaEspUploadManager::UpdateMode::HTTP);
	espUploadManager.httpUpdateNow();
	//	dwmUploadManager.checkNewFirmware(
	//			StringProcesses::stdStringToString(
	//					bbaGlobalSymbols::getDwmFirmwarePath()));
	webServer.begin(bbaGlobalSymbols::getWebServicePort());
	heartbeat.begin();

	//	std::string info = bbaESPInformations::getInformation();

	dwmGpiocontroller.begin();
	dwmGpiocontroller.startDwm();

	bbaDWMMessenger::registrationRequest();
}

// The loop function is called in an endless loop
void loop() {
	if (bbaGlobalSymbols::getIsRunning()) {
		serialHandler.process();
	}
	serverCommandHadler.process();

	webServer.handle();
	espUploadManager.otaHandle();

	heartbeat.checkHeartbeat();
}

#else

void setup() {
	Serial.begin(115200);
	while (!Serial) {
		Serial.print(".");
	}

	Serial.println(F("This test should produce the following:"));
	Serial.println(
			F("1 passed, 0 failed, 0 skipped, 0 timed out, out of 1 test(s)."));
	Serial.println(F("----"));
}

void loop() {
	tests tests;
	tests.runAll();
}

#endif
