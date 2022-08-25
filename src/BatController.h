#ifndef BATCONTROLLER_LIBRARY_BATCONTROLLER_H
#define BATCONTROLLER_LIBRARY_BATCONTROLLER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Display/Display.h>
#include <Input/InputI2C.h>
#include <Input/I2cExpander.h>
#include "Pins.hpp"

class BatControllerImpl {
public:
	BatControllerImpl();

	void begin();

	Display* getDisplay();
	InputI2C* getInput();

private:
	Display display;
	I2cExpander expander;
	InputI2C input;
};

extern BatControllerImpl BatController;

#endif //BATCONTROLLER_LIBRARY_BATCONTROLLER_H
