#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/LogicBlock.h>
#include "ILB_GridMob_A_V1.h"

class IWorldEntity;
class IEventListener;
namespace base {
	class WrappedWorldEntity;
	class EntityEvent;
};
namespace rpg3D {
	class IStatusEffectsModule;
};

namespace towerDefense_a_v1 {
namespace mobUnit {
class LB_GridMob_A_V1 : public rpg3D::LogicBlock, virtual public ILB_GridMob_A_V1 {priv typedef rpg3D::LogicBlock super;pub dCtor(LB_GridMob_A_V1);
	priv static const float AggroTargetMoveRange;

	priv std::shared_ptr<base::WrappedWorldEntity> aggroTarget = nullptr;

	priv float aggroDistance = 3.0f;
	priv float aggroLossDistance = 3.8f;

	priv bool isDirty_isEntityStunned = true;
	priv bool isEntityStunned = false;

	priv bool isLogicResultDirty = true;

    pub explicit LB_GridMob_A_V1();

	prot void onEntityChanged() override;
	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

	pub void logicTick(float deltaS) override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub ~LB_GridMob_A_V1() override;
};
};
};
