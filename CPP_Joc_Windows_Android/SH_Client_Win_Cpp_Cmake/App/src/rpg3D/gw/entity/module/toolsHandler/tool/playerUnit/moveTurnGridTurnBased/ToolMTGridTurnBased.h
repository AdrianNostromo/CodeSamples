#pragma once

#include <base/gh.h>
#include "ToolMTGTB_Base.h"
#include <base/math/Vector3.h>

class IMovingEntityModule;
class IEventListener;
namespace rpg3D {
	class IUserControlledModule;
	class IGPGrid2DContent;
	class IGrid2DContentModule;
};

namespace rpg3D {
namespace playerUnit {
class ToolMTGridTurnBased : public ToolMTGTB_Base {priv typedef ToolMTGTB_Base super;pub dCtor(ToolMTGridTurnBased);
	priv static std::string MOVEMENT_INITIATOR_IDENTIFIER;

	priv rpg3D::IUserControlledModule* userControlledModule = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;
	prot IGrid2DContentModule* grid2DContent = nullptr;

	prot rpg3D::IGPGrid2DContent* gpGrid2DContent = nullptr;

	priv ArrayList<std::shared_ptr<IEventListener>> movementAutoListenersList{};

	// This is persistent, don't reset it on trigger changes.
	priv int lastMove_turnBasedActionStateCounter = -1;

	pub explicit ToolMTGridTurnBased();

	prot void onEntityChanged() override;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	priv bool onFilterInteraction_FeetPlane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex);

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;

    pub ~ToolMTGridTurnBased() override;
};
};
};
