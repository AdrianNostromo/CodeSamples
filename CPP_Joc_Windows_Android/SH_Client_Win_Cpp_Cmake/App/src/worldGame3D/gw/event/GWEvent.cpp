#include "GWEvent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace base;

int GWEvent::typeCounter = 0;

GWEvent::GWEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void GWEvent::clearData() {
	voidData = nullptr;
	entityData = nullptr;
}

void* GWEvent::getVoidData() {
	return voidData;
}

GWEvent* GWEvent::setVoidData(void* voidData) {
	this->voidData = voidData;

	return this;
}

GWEvent* GWEvent::setLoopData(float deltaS, int deltaMS, float gameTimeS) {
	this->deltaS = deltaS;
	this->deltaMS = deltaMS;
	this->gameTimeS = gameTimeS;

	return this;
}

IWorldEntity* GWEvent::getEntityData() {
	return entityData;
}

GWEvent* GWEvent::setEntityData(IWorldEntity* entityData) {
	this->entityData = entityData;

	return this;
}

GWEvent::~GWEvent() {
	//void
}
