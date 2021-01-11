#pragma once

#include <base/gh.h>
#include <BulletDynamics/Dynamics/btActionInterface.h>

class btDiscreteDynamicsWorld;

namespace rpg3D {
ATTRIBUTE_ALIGNED16(class) IPActorControllerBase : public btActionInterface {
	pub virtual btDiscreteDynamicsWorld* getPWorld() = 0;
	pub virtual void setPWorld(btDiscreteDynamicsWorld* pWorld) = 0;

    pub ~IPActorControllerBase() override = default;
};
};
