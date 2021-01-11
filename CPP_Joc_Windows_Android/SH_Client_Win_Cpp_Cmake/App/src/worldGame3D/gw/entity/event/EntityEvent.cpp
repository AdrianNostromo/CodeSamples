#include "EntityEvent.h"

using namespace base;

int EntityEvent::typeCounter = 0;

EntityEvent::EntityEvent(bool isBubbleAsGWEntityEvent, std::string name)
	: isBubbleAsGWEntityEvent(isBubbleAsGWEntityEvent), name(std::move(name))
{
	type = ++typeCounter;
}

void EntityEvent::clearData() {
	voidData = nullptr;
	entityData = nullptr;
	entityDataOther = nullptr;
	wrappedValue = nullptr;

	initiatorIdentifier.clear();
}

void* EntityEvent::getVoidData() {
	return voidData;
}

EntityEvent* EntityEvent::setVoidData(void* voidData) {
	this->voidData = voidData;

	return this;
}

EntityEvent* EntityEvent::setWrappedValue(std::shared_ptr<IWrappedValue> wrappedValue) {
	this->wrappedValue = wrappedValue;

	return this;
}

IWorldEntity* EntityEvent::getEntityData() {
	return entityData;
}

IWorldEntity* EntityEvent::getEntityDataOther() {
	return entityDataOther;
}

EntityEvent* EntityEvent::setEntityData(IWorldEntity* entityData, IWorldEntity* entityDataOther) {
	this->entityData = entityData;
	this->entityDataOther = entityDataOther;

	return this;
}

EntityEvent* EntityEvent::setData_initiatorIdentifier(std::string& initiatorIdentifier) {
	this->initiatorIdentifier = initiatorIdentifier;

	return this;
}

EntityEvent::~EntityEvent() {
	//void
}
