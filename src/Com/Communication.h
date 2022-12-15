#ifndef BATCONTROLLER_LIBRARY_COMMUNICATION_H
#define BATCONTROLLER_LIBRARY_COMMUNICATION_H

#include <BatCommon.h>
#include <CommunicationCommon.h>
#include <Util/WithListeners.h>
#include "ComListener.h"
#include <DriveMode.h>

class Communication : public CommunicationCommon, private WithListeners<ComListener> {
public:
	Communication();
	~Communication() override;

	void begin();
	AsyncServer* getServer();

	void sendDriveMode(DriveMode mode);
	void sendVolume(uint8_t percent);

	/**
	 * @param dir Bitwise OR-ed values of direction buttons,
	 * lowest to highest bit represents forward, backward, left, right, respectively
	 */
	void sendDriveDir(uint8_t direction);
	void sendDriveSpeed(uint8_t percent);
	/**
	 * @param hue HSV hue divided by 2 [0-180]
	 */
	void sendBallHue(uint8_t hue);
	void sendHonk();
	void sendBoost(bool boost);
	void sendSettingsSound();
	void sendDisconnectRequest();

	void addListener(ComListener* listener);
	void removeListener(ComListener* listener);

private:
	bool isWiFiConnected() override;

	void processPacket(const ControlPacket& packet) override;

	AsyncServer* server = nullptr;
};

extern Communication Com;

#endif //BATCONTROLLER_LIBRARY_COMMUNICATION_H
