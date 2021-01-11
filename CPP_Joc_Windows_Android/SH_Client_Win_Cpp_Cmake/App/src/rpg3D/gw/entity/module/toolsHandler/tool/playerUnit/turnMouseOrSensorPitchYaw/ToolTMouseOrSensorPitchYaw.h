#pragma once

#include <base/gh.h>
#include "ToolTMOSPY_Base.h"

class IMovingEntityModule;
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class ToolTMouseOrSensorPitchYaw : public ToolTMOSPY_Base {priv typedef ToolTMOSPY_Base super;pub dCtor(ToolTMouseOrSensorPitchYaw);
	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;

	pub explicit ToolTMouseOrSensorPitchYaw();

	prot void onEntityChanged() override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~ToolTMouseOrSensorPitchYaw() override;
};
};
};
