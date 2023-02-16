#ifndef BATCONTROLLER_LIBRARY_COMLISTENER_H
#define BATCONTROLLER_LIBRARY_COMLISTENER_H


class ComListener {
public:
	virtual void onBattery(uint8_t percent, bool charging){};

	virtual void onSignalStrength(uint8_t percent){};

	virtual void onCameraError(uint8_t errorCode){};
};


#endif //BATCONTROLLER_LIBRARY_COMLISTENER_H
