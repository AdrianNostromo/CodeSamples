#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/LogicBlock.h>
#include "ILB_Mob_ImpactChaser_A_V1.h"

class IWorldEntity;
class IMovingEntityModule;
namespace base {
	class WrappedWorldEntity;
	class EntityEvent;
};
namespace rpg3D {
	class IStatusEffectsModule;
};

namespace rpg3D {
class LB_Mob_ImpactChaser_A_V1 : public rpg3D::LogicBlock, virtual public ILB_Mob_ImpactChaser_A_V1 {priv typedef rpg3D::LogicBlock super;pub dCtor(LB_Mob_ImpactChaser_A_V1);
	priv static const float AggroTargetMoveRange;

	priv const float aggroDistance;
	priv const float aggroLossDistance;

	priv std::shared_ptr<base::WrappedWorldEntity> aggroTarget = nullptr;
	priv IMovingEntityModule* movingEntity = nullptr;
	priv rpg3D::IStatusEffectsModule* statusEffects = nullptr;

	priv bool isDirty_isEntityStunned = true;
	priv bool isEntityStunned = false;

	priv bool isLogicResultDirty = true;

    pub explicit LB_Mob_ImpactChaser_A_V1(const float aggroDistance=3.0f, const float aggroLossDistance=3.8f);

	prot void onEntityChanged() override;
	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

	pub void logicTick(float deltaS) override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub ~LB_Mob_ImpactChaser_A_V1() override;
};
};
