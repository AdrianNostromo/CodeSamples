#pragma once

#include <base/gh.h>
#include "CCD_ChangeDetector.h"

namespace rpg3D {
class CustomCollisionDispatcher : public CCD_ChangeDetector {priv typedef CCD_ChangeDetector super;pub dCtor(CustomCollisionDispatcher);
	pub explicit CustomCollisionDispatcher(btCollisionConfiguration* collisionConfiguration);

	pub bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1) override;

	pub bool needsResponse(const btCollisionObject* body0, const btCollisionObject* body1) override;

	pub ~CustomCollisionDispatcher() override;
};
};
