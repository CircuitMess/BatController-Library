#include "BatController.h"

BatControllerImpl BatController;

BatControllerImpl::BatControllerImpl() : display(128, 128, -1, 0), input(&expander){
}

void BatControllerImpl::begin(){
	display.begin();
	display.getBaseSprite()->clear(TFT_BLACK);
	display.commit();

	expander.begin(0x74, 4, 5);
	expander.pinMode(BL_PIN, OUTPUT);
	expander.pinWrite(BL_PIN, true);

	input.preregisterButtons({ BTN_A, BTN_B, BTN_C, BTN_UP, BTN_DOWN, BTN_RIGHT, BTN_LEFT });
}

Display* BatControllerImpl::getDisplay(){
	return &display;
}

InputI2C* BatControllerImpl::getInput(){
	return &input;
}
