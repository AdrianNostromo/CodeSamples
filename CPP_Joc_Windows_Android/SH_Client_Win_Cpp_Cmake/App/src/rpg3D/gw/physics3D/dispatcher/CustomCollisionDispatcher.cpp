#include "CustomCollisionDispatcher.h"
#include <rpg3D/gw/physics3D/actor/IPhysicsActor.h>

using namespace rpg3D;

CustomCollisionDispatcher::CustomCollisionDispatcher(btCollisionConfiguration* collisionConfiguration)
	: super(collisionConfiguration)
{
	//void
}

bool CustomCollisionDispatcher::needsCollision(const btCollisionObject* body0, const btCollisionObject* body1) {
	bool ret = super::needsCollision(body0, body1);

	// This is what the body groups and mask are used for.
	// Returning false will disable collissions and events.

	// No need for logic here for now because the pBodies have groups and masks when they are added to the pWorld.
	// Example: pWorld->addRigidBody(configuredPBody->pBody, physicsGroupsBitMap, anyInteraction_maskBitMap);
	// The super call makes this check.

	return ret;
}

bool CustomCollisionDispatcher::needsResponse(const btCollisionObject* body0, const btCollisionObject* body1) {
	bool ret = super::needsResponse(body0, body1);

	// This is what ghost_mode controls.
	// Returning false will disable collissions but events will still be dispatched.

	IPhysicsActor* physicsActorA = static_cast<IPhysicsActor*>(body0->getUserPointer());
	IPhysicsActor* physicsActorB = static_cast<IPhysicsActor*>(body1->getUserPointer());

	if (physicsActorA == nullptr || physicsActorB == nullptr) {
		// PActors are removed after physics sim finishes. All pBodies should have pActors here.
		throw LogicException(LOC);
	}

	int physicsActorA_groupsBitMap = physicsActorA->getPhysicsGroupsBitMap();
	int physicsActorA_hardImpact_maskBitMap = physicsActorA->getHardImpact_maskBitMap();

	int physicsActorB_groupsBitMap = physicsActorB->getPhysicsGroupsBitMap();
	int physicsActorB_hardImpact_maskBitMap = physicsActorB->getHardImpact_maskBitMap();

	if ((physicsActorA_groupsBitMap & physicsActorB_hardImpact_maskBitMap) == 0
		|| (physicsActorB_groupsBitMap & physicsActorA_hardImpact_maskBitMap) == 0)
	{
		ret = false;
	}

	return ret;
}

CustomCollisionDispatcher::~CustomCollisionDispatcher() {
	//void
}
