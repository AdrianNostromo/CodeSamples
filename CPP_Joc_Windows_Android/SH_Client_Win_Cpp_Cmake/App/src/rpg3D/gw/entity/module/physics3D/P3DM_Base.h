#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IPhysics3DModule.h"

namespace rpg3D {
	class Physics3DTemplate;
};

namespace rpg3D {
class P3DM_Base : public EntityModule, virtual public IPhysics3DModule {priv typedef EntityModule super; pub dCtor(P3DM_Base);
	pub Physics3DTemplate* t; 

	pub P3DM_Base(
		IWorldEntity* entity,
		Physics3DTemplate* t);
	prot void createBMain() override;

	pub ~P3DM_Base() override;
};
};
