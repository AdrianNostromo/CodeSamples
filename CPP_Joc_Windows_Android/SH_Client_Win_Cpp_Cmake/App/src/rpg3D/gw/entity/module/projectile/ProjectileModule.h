#pragma once

#include <base/gh.h>
#include "PMBase.h"

namespace rpg3D {
class ProjectileModule : public PMBase {priv typedef PMBase super;pub dCtor(ProjectileModule);
    pub explicit ProjectileModule(
	    IWorldEntity* entity,
	    ProjectileTemplate* t);

	pub ~ProjectileModule() override;
};
};
