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

        client->onConnect([this](void*, AsyncClient* server){
            Serial.println("client connected to port");
        }, nullptr);

        client->onDisconnect([this](void*, AsyncClient* server){
            //TODO - reconnect to wifi in case of total disconnect
            Serial.println("client disconnected from port");
        });

        client->onError([this](void*, AsyncClient* c, int8_t error){
            Serial.print("error occured: ");
            Serial.println(c->errorToString(error));
        }, nullptr);

        client->onData([this](void* arg, AsyncClient* server, void* data, size_t len){
            Serial.printf("CLIENT DATA\n");
            Serial.printf("%s", data);
//            Serial.write((uint8_t *)data, len);
        });

        client->onTimeout([this](void*, AsyncClient*, uint32_t time){
            Serial.printf("timeout error %d passed\n", time);
        }, nullptr);

    }, nullptr);


    server->begin();
    Serial.printf("status: %d\n",server->status());
}

bool Communication::isWiFiConnected(){
    return true;
}
void Communication::processPacket(ControlPacket &packet){

}