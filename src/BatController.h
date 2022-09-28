#ifndef BATCONTROLLER_LIBRARY_BATCONTROLLER_H
#define BATCONTROLLER_LIBRARY_BATCONTROLLER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Display/Display.h>
#include <Input/InputGPIO.h>
#include "Pins.hpp"
#include "BatControllerDisplay.h"

class BatControllerImpl {
public:
	BatControllerImpl();

	void begin();

	Display* getDisplay();
	Input* getInput();

private:
	Display display;
	InputGPIO input;
};

extern BatControllerImpl BatController;

#endif //BATCONTROLLER_LIBRARY_BATCONTROLLER_H
