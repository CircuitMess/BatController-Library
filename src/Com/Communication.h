#ifndef BATCONTROLLER_LIBRARY_COMMUNICATION_H
#define BATCONTROLLER_LIBRARY_COMMUNICATION_H

#include <BatCommon.h>
#include <CommunicationCommon.h>
#include <Util/WithListeners.h>
#include "ComListener.h"

class Communication : public CommunicationCommon, private WithListeners<ComListener> {
public:
	Communication();
	~Communication();

	void begin();
	AsyncServer* getServer();

	void sendDriveMode();
	void sendVolume(uint8_t percent);
	void sendDriveDir(uint8_t hor, uint8_t ver);
	void sendDriveSpeed(uint8_t percent);
	void sendBallHue(uint8_t hue);
	void sendHonk();

	void addListener(ComListener* listener);
	void removeListener(ComListener* listener);

private:
	bool isWiFiConnected() override;

	void processPacket(const ControlPacket& packet) override;

	AsyncServer* server = nullptr;
};

extern Communication Com;

#endif //BATCONTROLLER_LIBRARY_COMMUNICATION_H
