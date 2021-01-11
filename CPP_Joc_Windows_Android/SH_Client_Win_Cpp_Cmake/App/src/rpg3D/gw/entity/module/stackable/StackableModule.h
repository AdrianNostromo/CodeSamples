#pragma once

#include <base/gh.h>
#include "SM_TeleportHandling.h"

namespace rpg3D {
class StackableModule : public SM_TeleportHandling {priv typedef SM_TeleportHandling super;
	pub dCtor(StackableModule);
    pub explicit StackableModule(
		IWorldEntity* entity,
		StackableTemplate* t);
	
    pub ~StackableModule() override;
};
};
