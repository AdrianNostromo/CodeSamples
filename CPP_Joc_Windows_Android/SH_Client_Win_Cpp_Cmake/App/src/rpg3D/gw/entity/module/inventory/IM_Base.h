#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IInventoryModule.h"

namespace rpg3D {
	class InventoryTemplate;
};

namespace rpg3D {
class IM_Base : public EntityModule, virtual public IInventoryModule {priv typedef EntityModule super; pub dCtor(IM_Base);
	pub InventoryTemplate* t;

	prot float goldAmount = 0;

	pub IM_Base(
		IWorldEntity* entity,
		InventoryTemplate* t);

	pub float getGoldAmount() final;
	pub float addGoldAmount(float goldAmountAddition) final;

	pub ~IM_Base() override;
};
};
