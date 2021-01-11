#pragma once

#include <base/gh.h>
#include "TSH_Throw.h"

class IMovingEntityModule;
namespace rpg3D {
	class IStackableModule;
};

namespace rpg3D {
class ToolStackablesHauler : public TSH_Throw {
	priv typedef TSH_Throw super;

	priv IStackableModule* stackable = nullptr;
	priv IMovingEntityModule* movingEntity = nullptr;
	
	pub dCtor(ToolStackablesHauler);
    pub explicit ToolStackablesHauler(ToolConfigStackablesHauler* config);

	prot void onEntityChanged() override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~ToolStackablesHauler() override;
};
};
