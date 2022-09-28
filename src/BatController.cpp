#include "BatController.h"
#include <Loop/LoopManager.h>

BatControllerImpl BatController;

BatControllerImpl::BatControllerImpl() : display(160, 128, -1, -3){
}

void BatControllerImpl::begin(){
	display.getTft()->setPanel(BatControllerDisplay::panel1());
	display.begin();
	display.getTft()->setRotation(3);
	display.swapBytes(false);
	display.clear(TFT_BLACK);
	display.commit();

	input.preregisterButtons({ BTN_A, BTN_B, BTN_LEFT, BTN_RIGHT });
	LoopManager::addListener(&input);

	pinMode(PIN_BL, OUTPUT);
	digitalWrite(PIN_BL, false);
}

Display* BatControllerImpl::getDisplay(){
	return &display;
}

Input* BatControllerImpl::getInput(){
	return &input;
}
