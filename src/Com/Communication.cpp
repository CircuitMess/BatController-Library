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
	if(packet.type == ComType::ShutdownAck){
		if(ackWait){
			ackWait = false;
			if(shutdownCallback) shutdownCallback(true);
		}
		return;
	}

	WithListeners<ComListener>::iterateListeners([packet](ComListener* listener){
		switch(packet.type){
			case ComType::Battery:
				listener->onBattery(packet.data, packet.data == UINT8_MAX);
				break;
			case ComType::SignalStrength:
				listener->onSignalStrength(packet.data);
				break;
			default:
				break;
		}
	});
}

void Communication::sendHonk(){
	ControlPacket packet{ ComType::Honk, 0 };
	sendPacket(packet);
}

void Communication::sendDriveMode(DriveMode mode){
	ControlPacket packet{ ComType::DriveMode, (uint8_t) mode };
	sendPacket(packet);
}

void Communication::sendVolume(uint8_t percent){
	ControlPacket packet{ ComType::Volume, percent };
	sendPacket(packet);
}

void Communication::sendDriveDir(uint8_t direction){
	ControlPacket packet{ ComType::DriveDir, direction };
	sendPacket(packet);
}

void Communication::sendDriveSpeed(uint8_t percent){
	ControlPacket packet{ ComType::DriveSpeed, percent };
	sendPacket(packet);
}

void Communication::sendBallHue(uint8_t hue){
	ControlPacket packet{ ComType::BallHue, hue };
	sendPacket(packet);
}

void Communication::sendBoost(bool boost){
	ControlPacket packet{ ComType::Boost, boost };
	sendPacket(packet);
}

void Communication::sendShutdown(std::function<void(bool)> callback){
	ControlPacket packet{ ComType::Shutdown, 0 };
	sendPacket(packet);
	ackWait = true;
	ackTimer = 0;
	shutdownCallback = callback;
}

void Communication::onLoop(uint micros){
	if(!ackWait) return;
	ackTimer += micros;

	if(ackTimer >= shutdownAckTimeout){
		ackWait = false;
		if(shutdownCallback) shutdownCallback(false);
	}
}
