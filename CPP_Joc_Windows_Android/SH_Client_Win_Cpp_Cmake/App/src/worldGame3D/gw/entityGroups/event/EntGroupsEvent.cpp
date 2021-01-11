#include "EntGroupsEvent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace worldGame3D;

int EntGroupsEvent::typeCounter = 0;

EntGroupsEvent::EntGroupsEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void EntGroupsEvent::clearData() {
	entsGroup = nullptr;
}

EntGroupsEvent* EntGroupsEvent::setData(IEntsGroup* entsGroup, IWorldEntity* entity) {
	this->entsGroup = entsGroup;
	this->entity = entity;

	return this;
}

EntGroupsEvent::~EntGroupsEvent() {
	//void
}
