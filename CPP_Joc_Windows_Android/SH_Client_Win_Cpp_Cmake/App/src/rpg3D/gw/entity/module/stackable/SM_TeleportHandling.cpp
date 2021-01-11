#include "SM_TeleportHandling.h"
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <functional>

using namespace rpg3D;

SM_TeleportHandling::SM_TeleportHandling(
	IWorldEntity* entity,
	StackableTemplate* t)
	: super(
		entity,
		t)
{
	//void
}

void SM_TeleportHandling::createMain() {
	super::createMain();

	if (t->canHaul) {
		e->addEventListener(
			IWorldEntity::EVENT_posManuallyChanged->type,
			std::bind(&SM_TeleportHandling::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList
		);
	}
}

void SM_TeleportHandling::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IWorldEntity::EVENT_posManuallyChanged->type) {
		IStackableModule* carryingStackable = getCarrying();
		if (carryingStackable != nullptr) {
			Vector3* pos = e->getPos();
			carryingStackable->getEntity()->setPos(pos->x, pos->y, pos->z + t->haulOffsetZM);
		}
	}

	else {
		throw LogicException(LOC);
	}
}

SM_TeleportHandling::~SM_TeleportHandling() {
	//void
}
