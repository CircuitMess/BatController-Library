#include <Arduino.h>
#include <CircuitOS.h>
#include "src/BatController.h"

void setup(){
	BatController.begin();

	BatController.getDisplay()->clear(TFT_RED);
	BatController.getDisplay()->commit();
}

void loop(){

}