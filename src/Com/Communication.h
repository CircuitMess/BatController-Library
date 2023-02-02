#ifndef BATCONTROLLER_LIBRARY_COMMUNICATION_H
#define BATCONTROLLER_LIBRARY_COMMUNICATION_H

#include <BatCommon.h>
#include <CommunicationCommon.h>
#include <Util/WithListeners.h>
#include "ComListener.h"
#include <DriveMode.h>
#include <DanceType.h>

enum class ComMode : uint8_t {
	Direct, External
};

class Communication : public CommunicationCommon, private WithListeners<ComListener> {
public:
	Communication();
	~Communication() override;

	void begin();
	AsyncServer* getServer();

	/**
	 * Sets Communication mode, defaults to Direct mode
	 * @param mode Direct mode (default) - host a WiFi AP, External mode - connect to an existing WiFi network
	 */
	void setMode(ComMode mode);

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
	void sendShutdown(std::function<void(bool)> callback);
    void sendDance(DanceType danceIndex);

	void addListener(ComListener* listener);
	void removeListener(ComListener* listener);

private:
	void onLoop(uint micros) override;
	bool isWiFiConnected() override;

	void processPacket(const ControlPacket& packet) override;

	AsyncServer* server = nullptr;

	static constexpr uint32_t shutdownAckTimeout = 3000000;
	std::function<void(bool)> shutdownCallback = nullptr;
	uint32_t ackTimer = 0;
	bool ackWait = false;

	ComMode mode = ComMode::Direct;
};

#endif //BATCONTROLLER_LIBRARY_COMMUNICATION_H
