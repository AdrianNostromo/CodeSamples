#include "CharacterPActorController.h"
#include <base/math/Math.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/physics3D/pBodyUpdateHandler/IPBodyUpdateHandler.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>

using namespace rpg3D;

CharacterPActorController::CharacterPActorController(
	CfgCharacterPActorController* cfg, IPBodyUpdateHandler* pBodyUpdateHandler)
	: cfg(cfg),
	pBodyUpdateHandler(pBodyUpdateHandler)
{
	//void
}

btDiscreteDynamicsWorld* CharacterPActorController::getPWorld() {
	return pWorld;
}

void CharacterPActorController::setPWorld(btDiscreteDynamicsWorld* pWorld) {
	this->pWorld = pWorld;
}

void CharacterPActorController::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) {
	//void
}

void CharacterPActorController::debugDraw(btIDebugDraw* debugDrawer) {
	//void
}

CharacterPActorController::~CharacterPActorController() {
	//void
}
