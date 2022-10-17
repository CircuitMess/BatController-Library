#include "Communication.h"
#include <NetworkConfig.h>


Communication Com;

Communication::Communication() {}
Communication::~Communication() {}

void Communication::begin() {
    delete server;
    server = new AsyncServer(port);

    server->onClient([this](void* arg, AsyncClient* client){
        Serial.println("new client connected");
        setClient(client);
    }, nullptr);

    server->begin();
    Serial.printf("status: %d\n",server->status());
}

bool Communication::isWiFiConnected(){
    //TODO: add and test this
//    return WiFi.softAPgetStationNum() == 1;
}
void Communication::processPacket(ControlPacket &packet){

}