#include "CCD_ChangeDetector.h"
#include <base/log/GlobalAccessLogHandler.h>
#include <base/exceptions/LogicException.h>
#include "../manifold/CustomPersistentManifold.h"
#include <rpg3D/gw/physics3D/actor/IPhysicsActor.h>

using namespace rpg3D;

CCD_ChangeDetector::CCD_ChangeDetector(btCollisionConfiguration* collisionConfiguration)
	: super(collisionConfiguration)
{
	//void
}

int CCD_ChangeDetector::getTotalFullContactsCount() {
	return totalFullContactsCount;
}

void CCD_ChangeDetector::onManifoldFullConnection(CustomPersistentManifold* manifold, bool isFullContact) {
	totalFullContactsCount += isFullContact ? 1 : -1;
	if (totalFullContactsCount < 0) {
		throw LogicException(LOC);
	}

	const btCollisionObject* pBodyA = manifold->getBody0();
	const btCollisionObject* pBodyB = manifold->getBody1();

	if (pBodyA == nullptr || pBodyB == nullptr) {
		throw LogicException(LOC);
	}

	IPhysicsActor* physicsActorA = static_cast<IPhysicsActor*>(pBodyA->getUserPointer());
	IPhysicsActor* physicsActorB = static_cast<IPhysicsActor*>(pBodyB->getUserPointer());

	// PActors may be nullptr here because this function is also called when a pBody is removed from the pWorld.
	if (physicsActorA != nullptr && physicsActorB != nullptr) {
		physicsActorA->getUsedInChangeContactsCount()++;
		physicsActorB->getUsedInChangeContactsCount()++;

		changesList.appendEmplace(
			physicsActorA->getWrapper(), physicsActorB->getWrapper(),
			isFullContact
		);
	}
}

CCD_ChangeDetector::~CCD_ChangeDetector() {
	//void
}
