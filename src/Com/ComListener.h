#ifndef BATCONTROLLER_LIBRARY_COMLISTENER_H
#define BATCONTROLLER_LIBRARY_COMLISTENER_H


class ComListener {
public:
	virtual void onBattery(uint8_t percent);
	virtual void onSignalStrength(uint8_t percent);
};


#endif //BATCONTROLLER_LIBRARY_COMLISTENER_H
