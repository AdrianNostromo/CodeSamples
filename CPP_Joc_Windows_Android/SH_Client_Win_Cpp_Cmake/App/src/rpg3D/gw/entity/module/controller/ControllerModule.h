#pragma once

#include <base/gh.h>
#include "CMLogicBlocks.h"

namespace rpg3D {
class ControllerModule : public CMLogicBlocks {priv typedef CMLogicBlocks super;pub dCtor(ControllerModule);
    pub explicit ControllerModule(
	    IWorldEntity* entity,
	    ControllerTemplate* t);

	pub ~ControllerModule() override;
};
};
