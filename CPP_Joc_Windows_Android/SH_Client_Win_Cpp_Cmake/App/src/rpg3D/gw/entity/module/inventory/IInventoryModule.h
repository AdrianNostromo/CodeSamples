#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>

namespace base {
	class EntityEvent;
};

namespace rpg3D {
class IInventoryModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub static base::EntityEvent* EVENT_inventory_goldAmountChanged;

	pub virtual float getGoldAmount() = 0;
	// Return the new gold amount.
	pub virtual float addGoldAmount(float goldAmountAddition) = 0;

    pub ~IInventoryModule() override = default;
};
};
