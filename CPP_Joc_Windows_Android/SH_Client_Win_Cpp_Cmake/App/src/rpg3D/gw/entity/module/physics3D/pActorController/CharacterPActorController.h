#pragma once

#include <base/gh.h>
#include "IPActorControllerBase.h"
#include <base/math/Vector3.h>
#include <btBulletDynamicsCommon.h>

namespace rpg3D {
	class IPBodyUpdateHandler;
	class CfgCharacterPActorController;

ATTRIBUTE_ALIGNED16(class) CharacterPActorController : virtual public IPActorControllerBase {pub dCtor(CharacterPActorController);
	prot CfgCharacterPActorController* cfg;
	priv IPBodyUpdateHandler* pBodyUpdateHandler;

	pub btDiscreteDynamicsWorld* pWorld = nullptr;

    pub explicit CharacterPActorController(
		CfgCharacterPActorController* cfg,
		IPBodyUpdateHandler* pBodyUpdateHandler);

	pub btDiscreteDynamicsWorld* getPWorld() final;
	pub void setPWorld(btDiscreteDynamicsWorld* pWorld) final;

	pub void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;

	pub void debugDraw(btIDebugDraw* debugDrawer) override;

    pub ~CharacterPActorController() override;
};
};
