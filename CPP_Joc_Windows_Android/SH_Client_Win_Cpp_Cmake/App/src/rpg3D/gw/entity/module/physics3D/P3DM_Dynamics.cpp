#include "P3DM_Dynamics.h"
#include "../../template/physics3D/Physics3DTemplate.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

P3DM_Dynamics::P3DM_Dynamics(
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

void P3DM_Dynamics::createMain() {
	super::createMain();

	//void
}

void P3DM_Dynamics::modifyLoopUsersCount(bool loopUserStateChange) {
	loopUsersCount += loopUserStateChange == true ? 1 : -1;
	if (loopUsersCount < 0 || loopUsersCount > 2) {
		throw LogicException(LOC);
	}

	if (loopUserStateChange && loopUsersCount == 1) {
		e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_prePhysicsSimulation->type,
			std::bind(&P3DM_Dynamics::onGWEvent_gameLoop_prePhysicsSimulation, this, std::placeholders::_1, std::placeholders::_2),
			loopListenersList
		);
	} else if (!loopUserStateChange && loopUsersCount == 0) {
		loopListenersList.clear();
	}
}

void P3DM_Dynamics::clearAllForces() {
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		configuredPBody->pBody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
		configuredPBody->pBody->clearForces();
	}
}

void P3DM_Dynamics::setLinearForce(
	float x, float y, float z, bool isContinuousUpdate, 
	float* linearDamping, bool applyToMovementPBodyOnly, 
	bool useCentralForceInsteadOfVelocitySet)
{
	if (linearDamping != nullptr) {
		pBodyMovement->setDamping(*linearDamping, pBodyMovement->getAngularDamping());
	}

	if (isContinuousUpdate) {
		if (!persistentLinearForce.isActive) {
			modifyLoopUsersCount(true);
		}

		persistentLinearForce.force.set(x, y, z);
		persistentLinearForce.applyToMovementPBodyOnly = applyToMovementPBodyOnly;
		persistentLinearForce.isActive = true;
	} else {
		if (persistentLinearForce.isActive) {
			modifyLoopUsersCount(false);
		}

		persistentLinearForce.isActive = false;

		btVector3 lVel{ x, y, z };
		if (applyToMovementPBodyOnly) {
			if (useCentralForceInsteadOfVelocitySet) {
				pBodyMovement->applyCentralForce(lVel);
			} else {
				pBodyMovement->setLinearVelocity(lVel);
			}

			if (!pBodyMovement->isActive()) {
				// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
				pBodyMovement->activate(true);
			}
		} else {
			for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
			    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

				if (useCentralForceInsteadOfVelocitySet) {
					configuredPBody->pBody->applyCentralForce(lVel);
				} else {
					configuredPBody->pBody->setLinearVelocity(lVel);
				}

				if (configuredPBody->pBody->getActivationState() != ACTIVE_TAG) {
					// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
					configuredPBody->pBody->activate(true);
				}
			}
		}
	}
}

void P3DM_Dynamics::setAngularVelocity(float x, float y, float z, bool isContinuousUpdate, float* angularDamping) {
	if (angularDamping != nullptr) {
		pBodyMovement->setDamping(pBodyMovement->getLinearDamping(), *angularDamping);
	}

	if (isContinuousUpdate) {
		if (!persistentAngularVelocity.isActive) {
			modifyLoopUsersCount(true);
		}

		persistentAngularVelocity.angularVelocity.set(x, y, z);
		persistentAngularVelocity.isActive = true;
	}
	else {
		if (persistentAngularVelocity.isActive) {
			modifyLoopUsersCount(false);
		}

		persistentAngularVelocity.isActive = false;

		btVector3 lVel{ x, y, z };
		pBodyMovement->setAngularVelocity(lVel);
	}

	if (!pBodyMovement->isActive()) {
		// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
		pBodyMovement->activate(true);
	}
}

void P3DM_Dynamics::setDamping(float linearDamping, float angularDamping) {
	pBodyMovement->setDamping(linearDamping, angularDamping);
}

void P3DM_Dynamics::setLinearDamping(float linearDamping) {
	if (!pBodyMovement->isActive()) {
		// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
		pBodyMovement->activate(true);
	}

	pBodyMovement->setDamping(linearDamping, pBodyMovement->getAngularDamping());
}

void P3DM_Dynamics::setAngularDamping(float angularDamping) {
	pBodyMovement->setDamping(pBodyMovement->getLinearDamping(), angularDamping);
}

void P3DM_Dynamics::applyImpulse(float x, float y, float z) {
	btVector3 v{ x, y, z };
	pBodyMovement->applyCentralImpulse(v);
}

void P3DM_Dynamics::onGWEvent_gameLoop_prePhysicsSimulation(IEventListener& eventListener, base::GWEvent& event) {
	if (persistentLinearForce.isActive) {
		btVector3 lVel{ persistentLinearForce.force.x, persistentLinearForce.force.y, persistentLinearForce.force.z };

		if (persistentLinearForce.applyToMovementPBodyOnly) {
			pBodyMovement->applyCentralForce(lVel);

			if (!pBodyMovement->isActive()) {
				// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
				pBodyMovement->activate(true);
			}
		} else {
			for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
			    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

				configuredPBody->pBody->applyCentralForce(lVel);

				if (configuredPBody->pBody->getActivationState() != ACTIVE_TAG) {
					// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
					configuredPBody->pBody->activate(true);
				}
			}
		}
	}
	if (persistentAngularVelocity.isActive) {
		btVector3 lVel{ persistentAngularVelocity.angularVelocity.x, persistentAngularVelocity.angularVelocity.y, persistentAngularVelocity.angularVelocity.z };

		pBodyMovement->setAngularVelocity(lVel);

		if (!pBodyMovement->isActive()) {
			// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
			pBodyMovement->activate(true);
		}
	}
}

P3DM_Dynamics::~P3DM_Dynamics() {
	//void
}
