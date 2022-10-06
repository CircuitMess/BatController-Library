#ifndef BATCONTROLLER_LIBRARY_COMMUNICATION_H
#define BATCONTROLLER_LIBRARY_COMMUNICATION_H

#include <CommunicationCommon.h>

class Communication : private CommunicationCommon{
public:
    Communication();
    ~Communication();
    void sendDriveMode();
    void sendVolume(uint8_t percent);
    void sendDriveDir(uint8_t hor, uint8_t ver);
    void sendDriveSpeed(uint8_t percent);
    void sendBallHue(uint8_t hue);
    void sendHonk();
    void begin();
    static Communication& getInstance();

private:
    AsyncServer* server;
    bool isWiFiConnected() override;
    void processPacket(ControlPacket &packet) override;
};

extern Communication Com;

#endif //BATCONTROLLER_LIBRARY_COMMUNICATION_H
