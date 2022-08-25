#include <Arduino.h>
#include <CircuitOS.h>
#include "src/BatControllerImpl.h"

void setup(){
	BatController.begin();

	BatController.getDisplay()->clear(TFT_RED);
	BatController.getDisplay()->commit();
}

void loop(){

}