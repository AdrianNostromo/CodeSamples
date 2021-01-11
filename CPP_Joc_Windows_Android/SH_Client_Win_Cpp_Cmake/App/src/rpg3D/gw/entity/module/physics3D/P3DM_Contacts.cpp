#include "P3DM_Contacts.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include "util/PhysicsEntityCallbackFilter.h"
#include "../../template/physics3D/Physics3DTemplate.h"
#include <rpg3D/gw/physics3D/actor/PhysicsActor.h>

using namespace rpg3D;

P3DM_Contacts::P3DM_Contacts(
	IWorldEntity* entity,
	Physics3DTemplate* t,
	int physicsGroupsBitMap,
	int anyInteraction_maskBitMap, int hardImpact_maskBitMap)
	:super(entity, t,
		physicsGroupsBitMap,
		anyInteraction_maskBitMap, hardImpact_maskBitMap)
{
	//void
}

void P3DM_Contacts::createMain() {
	super::createMain();

	physicsActor = new rpg3D::PhysicsActor(e, physicsGroupsBitMap, anyInteraction_maskBitMap, hardImpact_maskBitMap);

	if (e->getIsInGameWorldAndActive()) {
		IPhysicsActor* p = physicsActor;
		for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
			physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

			configuredPBody->pBody->setUserPointer(p);
		}
	}
}

btRigidBody* P3DM_Contacts::getPBodyVisualsAnchor() {
	return super::getPBodyVisualsAnchor();
}

IWorldEntity* P3DM_Contacts::getEntity() {
	return e;
}

IPhysicsEntityCallbackFilter* P3DM_Contacts::addEntityContactFilter(std::function<void(
	IPhysicsEntityCallbackFilter * filter, IWorldEntity * otherEntity, bool isContact)> cb_onContact)
{
	return physicsActor->addEntityContactFilter(cb_onContact);
}

void P3DM_Contacts::removeEntityContactFilter(IPhysicsEntityCallbackFilter* localFilter) {
	physicsActor->removeEntityContactFilter(localFilter);
}

void P3DM_Contacts::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	IPhysicsActor* p = physicsActor;
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBody->setUserPointer(p);
	}
}

void P3DM_Contacts::disconnectFromGamePartsPre() {
	if (physicsActor != nullptr) {
		physicsActor->disconnectAllContacts();
	}

	// Do this before the super call because the userPointer must be made nulltpr before the pBody is removed from the pWorld.
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBody->setUserPointer(nullptr);
	}

	super::disconnectFromGamePartsPre();
}

void P3DM_Contacts::disposePre() {
	if (physicsActor != nullptr) {
		physicsActor->reservedDisposePre();
	}

	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBody->setUserPointer(nullptr);
	}

	super::disposePre();
}

void P3DM_Contacts::disposeMain() {
	if (physicsActor != nullptr) {
		physicsActor->reservedDisposeMain();
		delete physicsActor;
		physicsActor = nullptr;
	}

	super::disposeMain();
}

P3DM_Contacts::~P3DM_Contacts() {
	//void
}
