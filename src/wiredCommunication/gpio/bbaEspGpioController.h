/*
 * bbaEspGpioController.h
 *
 *  Created on: 5 Eyl 2023
 *      Author: burak
 */

#ifndef WIREDCOMMUNICATION_GPIO_BBAESPGPIOCONTROLLER_H_
#define WIREDCOMMUNICATION_GPIO_BBAESPGPIOCONTROLLER_H_

#include <arduino.h>

class bbaEspGpioController {
private:
	bool getPinState(uint8_t pin);
	void setPinState(uint8_t pin, bool value);
	void togglePinState(uint8_t pin);

public:
	bbaEspGpioController();
	virtual ~bbaEspGpioController();

	void wifiLedToggle();
	void wifiLedOn();
	void wifiLedOff();
};

#endif /* WIREDCOMMUNICATION_GPIO_BBAESPGPIOCONTROLLER_H_ */
