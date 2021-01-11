#pragma once

#include <base/gh.h>
#include "ToolTGFDY_Base.h"

class IMovingEntityModule;
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class ToolTGlobalFocusDirectionYaw : public ToolTGFDY_Base {priv typedef ToolTGFDY_Base super;pub dCtor(ToolTGlobalFocusDirectionYaw);
	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;

	pub explicit ToolTGlobalFocusDirectionYaw();
	
	prot void onEntityChanged() override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~ToolTGlobalFocusDirectionYaw() override;
};
};
};
