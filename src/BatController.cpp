#include "BatController.h"
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include <Wire.h>
#include "Settings.h"
#include "BatteryService.h"

BatControllerImpl BatController;

BatControllerImpl::BatControllerImpl() : display(160, 128, -1, -3){
}

void BatControllerImpl::begin(bool backlight) {
	pinMode(PIN_BL, OUTPUT);
	digitalWrite(PIN_BL, HIGH);

	Wire.begin(I2C_SDA, I2C_SCL);
	SPIFFS.begin();
	Settings.begin();

	display.getTft()->setPanel(BatControllerDisplay::panel1());
    display.begin();
    display.getTft()->setRotation(1);
	display.clear(TFT_BLUE);
    display.commit();

	Battery.begin();

	input.preregisterButtons({ BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_A, BTN_B, BTN_C });
	LoopManager::addListener(&input);

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

	uint8_t dutyOn = mapDuty(Settings.get().screenBrightness);

	for(int i = 0; i <= 255; i++){
		uint8_t val = map(i, 0, 255, dutyOn, 255);
		ledcWrite(6, val);
		delay(d);
	}

    deinitPWM();
}

void BatControllerImpl::fadeIn(uint8_t d){
    if(!pwmInited){
        initPWM();
    }

	uint8_t dutyOn = mapDuty(Settings.get().screenBrightness);

	for(int i = 0; i <= 255; i++){
		uint8_t val = map(i, 0, 255, 255, dutyOn);
		ledcWrite(6, val);
		delay(d);
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
	return map(brightness, 0, 255, 240, 0);
}
