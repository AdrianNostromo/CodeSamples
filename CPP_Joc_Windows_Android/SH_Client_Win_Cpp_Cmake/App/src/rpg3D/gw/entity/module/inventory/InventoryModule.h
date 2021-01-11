#pragma once

#include <base/gh.h>
#include "IM_Base.h"

namespace rpg3D {
class InventoryModule : public IM_Base {priv typedef IM_Base super;pub dCtor(InventoryModule);
    pub explicit InventoryModule(
		IWorldEntity* entity,
		InventoryTemplate* t);

    pub ~InventoryModule() override;
};
};
