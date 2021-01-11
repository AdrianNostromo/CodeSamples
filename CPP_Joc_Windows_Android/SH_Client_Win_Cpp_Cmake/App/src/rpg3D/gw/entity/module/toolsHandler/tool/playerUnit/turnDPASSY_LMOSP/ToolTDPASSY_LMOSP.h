#pragma once

#include <base/gh.h>
#include "ToolTDPASSYLMOSP_Base.h"

class IMovingEntityModule;
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class ToolTDPASSY_LMOSP : public ToolTDPASSYLMOSP_Base {priv typedef ToolTDPASSYLMOSP_Base super;pub dCtor(ToolTDPASSY_LMOSP);
	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;

	pub explicit ToolTDPASSY_LMOSP();
	 
	prot void onEntityChanged() override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~ToolTDPASSY_LMOSP() override;
};
};
};
