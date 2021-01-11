#pragma once

#include <base/gh.h>
#include "ToolMLS_Base.h"

class IMovingEntityModule;
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class ToolMLocalStrafe : public ToolMLS_Base {priv typedef ToolMLS_Base super;pub dCtor(ToolMLocalStrafe);
	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;

	pub explicit ToolMLocalStrafe();

	prot void onEntityChanged() override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~ToolMLocalStrafe() override;
};
};
};
