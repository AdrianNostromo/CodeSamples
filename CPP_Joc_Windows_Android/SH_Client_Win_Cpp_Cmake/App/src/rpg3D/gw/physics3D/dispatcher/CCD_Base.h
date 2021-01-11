#pragma once

#include <base/gh.h>
#include "../manifold/util/IManifoldHandler.h"
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>

namespace rpg3D {
class CCD_Base : public btCollisionDispatcher, virtual public IManifoldHandler {priv typedef btCollisionDispatcher super;pub dCtor(CCD_Base);
	pub explicit CCD_Base(btCollisionConfiguration* collisionConfiguration);

	pub btPersistentManifold* getNewManifold(const btCollisionObject* b0, const btCollisionObject* b1) override;
	pub void releaseManifold(btPersistentManifold* manifold) override;

	pub ~CCD_Base() override;
};
};
