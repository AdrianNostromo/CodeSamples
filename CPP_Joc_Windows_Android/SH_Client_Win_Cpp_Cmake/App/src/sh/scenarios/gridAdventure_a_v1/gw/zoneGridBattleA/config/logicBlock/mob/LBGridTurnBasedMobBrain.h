#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/LogicBlock.h>
#include "ILBGridTurnBasedMobBrain.h"
#include <rpg3D/gw/turnBasedPlay/util/ITurnBasedEntity.h>

class IMovingEntityModule;
namespace rpg3D {
	class IToolsHandlerModule;
	class IGPTurnBasedPlay;
	class IGPGrid2DContent;
	class IGrid2DContentModule;
	class ITeamModule;
	class ITurnBasedInteractor;
};

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class LBGridTurnBasedMobBrain : public rpg3D::LogicBlock, public virtual ILBGridTurnBasedMobBrain, public virtual rpg3D::ITurnBasedEntity {priv typedef rpg3D::LogicBlock super;pub dCtor(LBGridTurnBasedMobBrain);
    prot rpg3D::IToolsHandlerModule* toolsHandler = nullptr;
	prot rpg3D::IGrid2DContentModule* grid2DContent = nullptr;
	prot IMovingEntityModule* movingEntity = nullptr;
	prot rpg3D::ITeamModule* team = nullptr;

    prot rpg3D::IGPTurnBasedPlay* gpGridTurnBased = nullptr;
	prot rpg3D::IGPGrid2DContent* gpGrid2DContent = nullptr;

	prot rpg3D::ITurnBasedInteractor* turnBasedInteractor = nullptr;
    
    pub explicit LBGridTurnBasedMobBrain();

	prot void onEntityChanged() override;
	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

	// Call this after turnBasedInteractor->currentActionType or startedActionType changed.
	priv virtual void onActiveActionInfoChanged();

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;

	pub int getTeam() final;

	pub void onCurrentActionChanged() override;

    pub ~LBGridTurnBasedMobBrain() override;
};
};
};
