#include "Communication.h"
#include <NetworkConfig.h>
#include <esp_wifi_types.h>
#include <esp_wifi.h>

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
		return (WiFi.softAPgetStationNum() == 1);
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
	}

	WithListeners<ComListener>::iterateListeners([packet, this](ComListener* listener){
		switch(packet.type){
			case ComType::Battery:
				listener->onBattery(packet.data, packet.data == UINT8_MAX);
				break;
			case ComType::SignalStrength:
				if(mode == ComMode::Direct){
					listener->onSignalStrength(packet.data);
				}else if(mode == ComMode::External){
					listener->onSignalStrength(std::min(this->getSignalStrength(), packet.data));
				}
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

void Communication::sendDance(DanceType danceIndex) {
    ControlPacket packet{ ComType::Dance, (uint8_t)danceIndex };
    sendPacket(packet);
}

void Communication::onLoop(uint micros){
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

uint8_t Communication::getSignalStrength(){
	wifi_ap_record_t info;
	uint8_t percentage = 0;

	if(esp_wifi_sta_get_ap_info(&info) == ESP_OK){
		auto con = constrain(info.rssi, MinSS, MaxSS);
		percentage = map(con, MinSS, MaxSS, 0, 100);
	}

	return percentage;
}
