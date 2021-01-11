#pragma once

#include <base/gh.h>
#include "LBGTBAT_Base.h"
#include <rpg3D/gw/turnBasedPlay/util/ITurnBasedEntity.h>
#include <rpg3D/gw/turnBasedPlay/util/ITurnBasedInteractor.h>

namespace base {
    class GWEvent;
};
namespace rpg3D {
    class IGPTurnBasedPlay;
    class IGPGrid2DContent;
    class IGrid2DContentModule;
    class ITeamModule;
};

namespace rpg3D {
namespace playerUnit {
class LBGTBAT_TurnMonitor : public LBGTBAT_Base, public virtual ITurnBasedEntity {priv typedef LBGTBAT_Base super;pub dCtor(LBGTBAT_TurnMonitor);
    priv IGrid2DContentModule* grid2DContent = nullptr;
    priv rpg3D::ITeamModule* team = nullptr;

    prot rpg3D::IGPTurnBasedPlay* gpGridTurnBased = nullptr;
	prot rpg3D::IGPGrid2DContent* gpGrid2DContent = nullptr;

    priv rpg3D::ITurnBasedInteractor* turnBasedInteractor = nullptr;
    // This is used to know if the action is started or not. Used to make sure a single action is started and so show valid_movement_tiles.
    priv ITurnBasedInteractor::ActionType startedActionType = ITurnBasedInteractor::ActionType::None;

    prot ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};
    
    pub explicit LBGTBAT_TurnMonitor();

    prot bool shouldManageConnectTool(std::shared_ptr<util::WrappedTool> wTool) override;

    prot void onEntityChanged() override;
    prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

    prot void onEvent(IEventListener& eventListener, base::EntityEvent& event);

    // Call this after turnBasedInteractor->currentActionType or startedActionType changed.
    priv void onActiveActionInfoChanged();

    pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
    pub void disconnectFromGamePartsPre() override;
    
    pub int getTeam() final;

    pub void onCurrentActionChanged() final;

    pub ~LBGTBAT_TurnMonitor() override;
};
};
};
