#include "Communication.h"
#include <NetworkConfig.h>


Communication Com;

Communication::Communication(){}

Communication::~Communication(){}

void Communication::begin(){
	server = new AsyncServer(controlPort);

	server->onClient([this](void* arg, AsyncClient* client){
		setClient(std::unique_ptr<AsyncClient>(client));
	}, nullptr);

	server->begin();
}

void Communication::addListener(ComListener* listener){
	WithListeners<ComListener>::addListener(listener);
}

void Communication::removeListener(ComListener* listener){
	WithListeners<ComListener>::removeListener(listener);
}

bool Communication::isWiFiConnected(){
	return WiFi.softAPgetStationNum() == 1;
}

void Communication::processPacket(const ControlPacket& packet){
	if(packet.type != ComType::Battery) return;
	WithListeners<ComListener>::iterateListeners([](ComListener* listener){
		listener->onBattery();
	});
}

void Communication::sendHonk(){
	ControlPacket packet;
	packet.type = ComType::Honk;
	packet.data = 0;
	sendPacket(packet);
}