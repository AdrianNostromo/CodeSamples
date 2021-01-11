#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/LogicBlock.h>
#include "ILB_Turret_CubesCannon_A.h"

class IWorldEntity;
class IEventListener;
namespace base {
	class WrappedWorldEntity;
	class EntityEvent;
};
namespace towerDefense_a_v1 {
	class ICustomCubesCannonTurretModule;
};
namespace rpg3D {
	class IStatusEffectsModule;
};

namespace towerDefense_a_v1 {
namespace mobUnit {
class LB_Turret_CubesCannon_A : public rpg3D::LogicBlock, virtual public ILB_Turret_CubesCannon_A {priv typedef rpg3D::LogicBlock super;pub dCtor(LB_Turret_CubesCannon_A);
	pub class Phases {
		pub static const int MANAGED_RELOAD;
		pub static const int MANAGED_AIM;
		pub static const int MANAGED_FIRE_PROJECTILE_AT_TARGET;
		pub static const int POST_PROJECTILE_FIRE_COOLDOWN;
	};

	priv rpg3D::IStatusEffectsModule* statusEffects = nullptr;
	priv ICustomCubesCannonTurretModule* customCubesCannonTurret = nullptr;

	priv const float aggroDistance = 2.0f;
	priv const float aggroLossDistance = 2.0f;
	
	priv float cooldownS = 0.0f;

	priv bool isEventReloadFinished = false;
	priv bool isProjectileLaunched = false;

	priv std::shared_ptr<base::WrappedWorldEntity> aggroTarget = nullptr;

	priv int phase = Phases::MANAGED_RELOAD;
	pub bool isPhaseInit = false;

    pub explicit LB_Turret_CubesCannon_A();

	prot void onEntityChanged() override;
	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

	pub void logicTick(float deltaS) override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	priv int tickPhase(float deltaS);

	pub ~LB_Turret_CubesCannon_A() override;
};
};
};
