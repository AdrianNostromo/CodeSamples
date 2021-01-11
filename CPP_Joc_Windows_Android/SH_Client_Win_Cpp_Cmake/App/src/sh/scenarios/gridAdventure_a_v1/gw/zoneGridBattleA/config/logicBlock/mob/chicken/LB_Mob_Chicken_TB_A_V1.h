#pragma once

#include <base/gh.h>
#include "../LBGridTurnBasedMobBrain.h"
#include "ILB_Mob_Chicken_TB_A_V1.h"
#include <rpg3D/gw/turnBasedPlay/util/ITurnBasedInteractor.h>
#include <string>

class IWorldEntity;
class IEventListener;
namespace base {
	class EntityEvent;
};

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class LB_Mob_Chicken_TB_A_V1 : public LBGridTurnBasedMobBrain, virtual public ILB_Mob_Chicken_TB_A_V1 {priv typedef LBGridTurnBasedMobBrain super;pub dCtor(LB_Mob_Chicken_TB_A_V1);
	priv static std::string LOCAL_ACTION_INITIATOR_IDENTIFIER;

	priv bool isActionStartEventReceived = false;
	// This is used to know if the action is started or not. Used to make sure a single action is started and so show valid_movement_tiles.
	priv rpg3D::ITurnBasedInteractor::ActionType startedActionType = rpg3D::ITurnBasedInteractor::ActionType::None;
	// This is used to detect actions that never finish bugs.
	priv float startedActionDurationS = 0.0f;

	priv ArrayList<std::shared_ptr<IEventListener>> currentActionAutoListenersList{};

	priv int nextPatrollMoveDirectionX = 1;

    pub explicit LB_Mob_Chicken_TB_A_V1();

	prot void onEntityChanged() override;
	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;

	priv void onActiveActionInfoChanged() override;

	pub void logicTick(float deltaS) override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub void onCurrentActionChanged() override;

	pub ~LB_Mob_Chicken_TB_A_V1() override;
};
};
};
