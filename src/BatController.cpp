#include "BatController.h"
#include <Loop/LoopManager.h>

BatControllerImpl BatController;

BatControllerImpl::BatControllerImpl() : display(160, 128, -1, -3){
}

void BatControllerImpl::begin(bool backlight) {
	display.getTft()->setPanel(BatControllerDisplay::panel1());
    display.begin();
    display.getTft()->setRotation(1);
	display.clear(TFT_BLUE);
    display.commit();

    //TODO: add input
	//input.preregisterButtons({ BTN_A, BTN_B, BTN_LEFT, BTN_RIGHT });
	//LoopManager::addListener(&input);

	pinMode(PIN_BL, OUTPUT);
	digitalWrite(PIN_BL, false);

    if(backlight){
        fadeIn();
    }
}

Display* BatControllerImpl::getDisplay(){
	return &display;
}

Input* BatControllerImpl::getInput(){
	return &input;
}



void BatControllerImpl::setBrightness(uint8_t brightness){
    if(!pwmInited){
        initPWM();
    }

    ledcWrite(6, mapDuty(brightness));
}

bool BatControllerImpl::backlightPowered() const{
    return pwmInited;
}

void BatControllerImpl::backlightOff(){
    deinitPWM();
}


void BatControllerImpl::fadeOut(uint8_t d){
    if(!pwmInited){
        initPWM();
    }
    deinitPWM();
}

void BatControllerImpl::fadeIn(uint8_t d){
    if(!pwmInited){
        initPWM();
    }

}

void BatControllerImpl::initPWM() {
    ledcSetup(6, 5000, 8);
    ledcAttachPin(PIN_BL, 6);
    pwmInited = true;
}

void BatControllerImpl::deinitPWM() {
    ledcDetachPin(PIN_BL);
    digitalWrite(PIN_BL, HIGH);
    pwmInited = false;
}

uint8_t BatControllerImpl::mapDuty(uint8_t brightness) {
    return 0;
}
