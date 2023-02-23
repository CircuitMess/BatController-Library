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
	 * @param direction Bitwise OR-ed values of direction buttons, using only the lowest 4 bits.
	 * Lowest to highest bit represents forward, backward, left, right, respectively
	 * @param speed Lowest 4 bits indicate driving speed (0 - 15), used for rough speed estimates, otherwise sendDriveSpeed() is preferred.
	 * If speed is zero, then a default value is used.
	 */
	void sendDriveDir(uint8_t direction, uint8_t speed = 0);
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
	void sendHeadlights(uint8_t val);
	void sendTaillights(uint8_t val);
	void sendUnderlights(uint8_t color);
	void sendSoundEffect(uint8_t sample);
	void sendOverrideSound(bool manual);
	void sendIdleSound(bool toggle);
	/**
	 * Send duration after which all Motors functions will be locked until a MotorsTimeoutClear packet is received
	 * @param timeout Duration of timeout, increment is 100ms
	 */
	void sendMotorsTimeout(uint8_t timeout);
	/**
	 * Unlocks Motors functions that could have been blocked by a MotorsTimeout packet
	 */
	void sendMotorsTimeoutClear();

	void addListener(ComListener* listener);
	void removeListener(ComListener* listener);

private:
	void onLoop(uint micros) override;
	bool isWiFiConnected() override;

	void onConnect() override;
	void onDisconnect() override;

	void processPacket(const ControlPacket& packet) override;
	uint8_t getSignalStrength();

	AsyncServer* server = nullptr;

	static constexpr uint32_t shutdownAckTimeout = 3000000;
	std::function<void(bool)> shutdownCallback = nullptr;
	uint32_t ackTimer = 0;
	bool ackWait = false;

	static constexpr int8_t MaxSS = -50;
	static constexpr int8_t MinSS = -90;

	ComMode mode = ComMode::Direct;

	uint8_t signalStrengthReceived = 0;
	uint32_t signalStrengthTime = 0;
	static constexpr uint32_t signalStrengthTimeout = 5000000;
};

extern Communication Com;

#endif //BATCONTROLLER_LIBRARY_COMMUNICATION_H
