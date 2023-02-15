#include "Communication.h"
#include <NetworkConfig.h>

Communication::Communication(){
	WithListeners<ComListener>::reserve(16);
}

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
	if(mode == ComMode::Direct){
		if(signalStrengthTime < signalStrengthTimeout){
			return (WiFi.softAPgetStationNum() == 1);
		}else{
			bool signalStrengthCheck = (signalStrengthReceived > 0);
			signalStrengthTime = 0;
			signalStrengthReceived = 0;

			if(!signalStrengthCheck) Serial.println("signal strength check failed");

			return (WiFi.softAPgetStationNum() == 1) && signalStrengthCheck;
		}
	}else if(mode == ComMode::External){
		return WiFi.isConnected();
	}
}

void Communication::processPacket(const ControlPacket& packet){
	if(packet.type == ComType::ShutdownAck){
		if(ackWait){
			ackWait = false;
			if(shutdownCallback) shutdownCallback(true);
		}
		return;
	}else if(packet.type == ComType::SignalStrength){
		signalStrengthReceived++;
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

void Communication::sendSettingsSound(){
	ControlPacket packet{ ComType::SettingsSound, 0 };
	sendPacket(packet);
}

void Communication::sendDisconnectRequest(){
	ControlPacket packet{ ComType::Disconnect, 0 };
	sendPacket(packet);
}

void Communication::sendHeadlights(uint8_t val){
	ControlPacket packet{ ComType::Headlights, val };
	sendPacket(packet);
}

void Communication::sendTaillights(uint8_t val){
	ControlPacket packet{ ComType::Taillights, val };
	sendPacket(packet);
}

void Communication::sendUnderlights(uint8_t color){
	ControlPacket packet{ ComType::Underlights, color };
	sendPacket(packet);
}


void Communication::sendSoundEffect(uint8_t sample){
	ControlPacket packet{ ComType::SoundEffect, sample };
	sendPacket(packet);
}

void Communication::sendShutdown(std::function<void(bool)> callback){
	ControlPacket packet{ ComType::Shutdown, 0 };
	sendPacket(packet);
	ackWait = true;
	ackTimer = 0;
	shutdownCallback = callback;
}

void Communication::sendDance(DanceType danceIndex){
	ControlPacket packet{ ComType::Dance, (uint8_t) danceIndex };
	sendPacket(packet);
}

void Communication::sendOverrideSound(bool manual){
	ControlPacket packet{ ComType::OverrideSound, manual};
	sendPacket(packet);
}

void Communication::onLoop(uint micros){
	signalStrengthTime += micros;

	if(!ackWait) return;
	ackTimer += micros;

	if(ackTimer >= shutdownAckTimeout){
		ackWait = false;
		if(shutdownCallback) shutdownCallback(false);
	}
}

void Communication::setMode(ComMode mode){
	if(this->mode == mode) return;

	if(isConnected()){
		end();
	}
	this->mode = mode;
	begin();
}

void Communication::onConnect(){
	signalStrengthReceived = 0;
	signalStrengthTime = 0;
}

void Communication::onDisconnect(){
	signalStrengthReceived = 0;
	signalStrengthTime = 0;
}
