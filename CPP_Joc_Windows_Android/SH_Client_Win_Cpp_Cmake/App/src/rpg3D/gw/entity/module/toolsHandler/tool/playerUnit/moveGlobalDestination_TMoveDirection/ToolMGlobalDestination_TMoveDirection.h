#pragma once

#include <base/gh.h>
#include "ToolMGDTMD_Base.h"

class IMovingEntityModule;
namespace rpg3D {
	class IUserControlledModule;
	class IGPGrid2DContent;
};

namespace rpg3D {
namespace playerUnit {
class ToolMGlobalDestination_TMoveDirection : public ToolMGDTMD_Base {priv typedef ToolMGDTMD_Base super;pub dCtor(ToolMGlobalDestination_TMoveDirection);
	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;

	prot rpg3D::IGPGrid2DContent* gpGrid2DContent = nullptr;

	pub explicit ToolMGlobalDestination_TMoveDirection(int movementTypeOverride, bool usesGridMovement, DestinationType destinationType);

	priv bool customFilterCheck_worldPos(Vector3& worldPos);
	prot void onEntityChanged() override;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;

    pub ~ToolMGlobalDestination_TMoveDirection() override;
};
};
};
