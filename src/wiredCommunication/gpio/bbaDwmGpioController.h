/*
 * dwmGpioController.h
 *
 *  Created on: Feb 14, 2023
 *      Author: aress
 */

#ifndef WIREDCOMMUNICATION_GPIO_BBADWMGPIOCONTROLLER_H_
#define WIREDCOMMUNICATION_GPIO_BBADWMGPIOCONTROLLER_H_

class bbaDwmGpioController final {
public:
	bbaDwmGpioController();
	virtual ~bbaDwmGpioController();
	void begin();

	void stopDwm();
	void startDwm();
	void resetDwm();
};

#endif /* WIREDCOMMUNICATION_GPIO_BBADWMGPIOCONTROLLER_H_ */
