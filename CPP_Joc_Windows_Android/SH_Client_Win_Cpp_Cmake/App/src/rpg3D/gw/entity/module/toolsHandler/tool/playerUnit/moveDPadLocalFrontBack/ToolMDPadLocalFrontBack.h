#pragma once

#include <base/gh.h>
#include "ToolMDPLFB_Base.h"

class IMovingEntityModule;
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class ToolMDPadLocalFrontBack : public ToolMDPLFB_Base {priv typedef ToolMDPLFB_Base super;pub dCtor(ToolMDPadLocalFrontBack);
	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;
	
	pub explicit ToolMDPadLocalFrontBack();

	prot void onEntityChanged() override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~ToolMDPadLocalFrontBack() override;
};
};
};
