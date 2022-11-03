#include "Communication.h"
#include <NetworkConfig.h>


Communication Com;

Communication::Communication(){}

Communication::~Communication(){}

void Communication::begin(){
	if(server) return;

	server = new AsyncServer(controlPort);
	server->begin();
}

AsyncServer* Communication::getServer(){
	return server;
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
	ControlPacket packet{ComType::Honk, 0};
	sendPacket(packet);
}

void Communication::sendDriveMode(DriveMode mode){
	ControlPacket packet{ComType::DriveMode, (uint8_t)mode};
	sendPacket(packet);
}

void Communication::sendVolume(uint8_t percent){
	ControlPacket packet{ComType::DriveMode, percent};
	sendPacket(packet);
}

void Communication::sendDriveDir(uint8_t direction){
	ControlPacket packet{ComType::DriveMode, direction};
	sendPacket(packet);
}

void Communication::sendDriveSpeed(uint8_t percent){
	ControlPacket packet{ComType::DriveMode, percent};
	sendPacket(packet);
}

void Communication::sendBallHue(uint8_t hue){
	ControlPacket packet{ComType::DriveMode, hue};
	sendPacket(packet);
}

void Communication::sendBoost(bool boost){
	ControlPacket packet{ComType::DriveMode, boost};
	sendPacket(packet);
}
