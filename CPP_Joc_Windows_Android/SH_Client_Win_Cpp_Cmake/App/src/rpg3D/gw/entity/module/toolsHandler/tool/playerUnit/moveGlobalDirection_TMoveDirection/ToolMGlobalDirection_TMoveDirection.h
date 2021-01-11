#pragma once

#include <base/gh.h>
#include "ToolMGDiTMD_Base.h"

class IMovingEntityModule;
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class ToolMGlobalDirection_TMoveDirection : public ToolMGDiTMD_Base {priv typedef ToolMGDiTMD_Base super;pub dCtor(ToolMGlobalDirection_TMoveDirection);
	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;

	pub explicit ToolMGlobalDirection_TMoveDirection();

	prot void onEntityChanged() override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~ToolMGlobalDirection_TMoveDirection() override;
};
};
};
