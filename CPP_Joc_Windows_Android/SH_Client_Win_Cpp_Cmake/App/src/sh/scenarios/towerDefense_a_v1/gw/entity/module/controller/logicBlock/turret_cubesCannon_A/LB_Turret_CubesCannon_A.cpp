#include <base/math/Math.h>
#include "LB_Turret_CubesCannon_A.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/template/statusEffects/StatusEffectsTemplate.h>
#include <rpg3D/gw/entity/module/statusEffects/IStatusEffectsModule.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectStun.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/EntityIds.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <sh/scenarios/towerDefense_a_v1/gw/entity/module/customCubesCannonTurret/ICustomCubesCannonTurretModule.h>
#include <rpg3D/gw/entity/module/payloadReceiverTarget/PayloadReceiverTargetModule.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/event/listener/IEventListener.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::mobUnit;

const int LB_Turret_CubesCannon_A::Phases::MANAGED_RELOAD = 1;
const int LB_Turret_CubesCannon_A::Phases::MANAGED_AIM = 2;
const int LB_Turret_CubesCannon_A::Phases::MANAGED_FIRE_PROJECTILE_AT_TARGET = 3;
const int LB_Turret_CubesCannon_A::Phases::POST_PROJECTILE_FIRE_COOLDOWN = 4;

LB_Turret_CubesCannon_A::LB_Turret_CubesCannon_A()
	: super(0.1f/*tickCooldownS*/)
{
	//void
}

void LB_Turret_CubesCannon_A::onEntityChanged() {
	super::onEntityChanged();

	if (e == nullptr) {
		statusEffects = nullptr;
		customCubesCannonTurret = nullptr;
	} else {
		statusEffects = e->getComponentAs<rpg3D::IStatusEffectsModule*>(true/*mustExist*/);
		customCubesCannonTurret = e->getComponentAs<ICustomCubesCannonTurretModule*>(true/*mustExist*/);
	}
}

void LB_Turret_CubesCannon_A::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		// Events with autoListenersList_whileActive are automatically cleaned up in this case.

		//void
	} else {
		e->addEventListener(
			rpg3D::IStatusEffectsModule::EVENT_statusEffects_changed->type, ICustomCubesCannonTurretModule::EVENT_ReloadFinished->type,
			std::bind(&LB_Turret_CubesCannon_A::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList_whileActive
		);
		e->addEventListener(
			ICustomCubesCannonTurretModule::EVENT_ProjectileLaunched->type, ICustomCubesCannonTurretModule::EVENT_ProjectileLaunchCancelled_TargetLost->type,
			std::bind(&LB_Turret_CubesCannon_A::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList_whileActive
		);
	}
}

void LB_Turret_CubesCannon_A::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IStatusEffectsModule::EVENT_statusEffects_changed->type) {
		//void
	}else if (event.type == ICustomCubesCannonTurretModule::EVENT_ReloadFinished->type) {
		isEventReloadFinished = true;
	}else if (event.type == ICustomCubesCannonTurretModule::EVENT_ProjectileLaunched->type) {
		isProjectileLaunched = true;
	}else if (event.type == ICustomCubesCannonTurretModule::EVENT_ProjectileLaunchCancelled_TargetLost->type) {
		isProjectileLaunched = true;
	}else {
		throw LogicException(LOC);
	}
}

void LB_Turret_CubesCannon_A::logicTick(float deltaS) {
	super::logicTick(deltaS);

	int newPhase = tickPhase(deltaS);
	if (newPhase != phase) {
		phase = newPhase;
		isPhaseInit = false;
	} else if(!isPhaseInit) {
		isPhaseInit = true;
	}
}

int LB_Turret_CubesCannon_A::tickPhase(float deltaS) {
	switch (phase) {
	case Phases::MANAGED_RELOAD: {
        if (!isPhaseInit) {
			if (customCubesCannonTurret->getIsProjectileLoaded()) {
				return Phases::MANAGED_AIM;
			}
			// Do a reload action.
			isEventReloadFinished = false;

			customCubesCannonTurret->addAction_ActionReload();
		} else {
			// Wait for a reload_finished or already_loaded event.
			if (isEventReloadFinished) {
				return Phases::MANAGED_AIM;
			}
		}

		//void

        break;
    }
	case Phases::MANAGED_AIM: {
        if (!isPhaseInit) {
            //void
		} else {
			//void
		}

		bool isAggroTargetChanged = false;

		if (aggroTarget != nullptr && aggroTarget->entity == nullptr) {
			aggroTarget = nullptr;

			isAggroTargetChanged = true;
		}

		if (aggroTarget == nullptr) {
			// Get the closest mob entity in aggro range.
			IWorldEntity* closestMobEntity = nullptr;
			float closestMobEntityDist = -1.0f;
			std::shared_ptr<ArrayList<IWorldEntity*>> mobsList = e->getGW()->getMain()->getEntitiesListWithInstanceId(*EntityIds::gridMob);
			if (mobsList != nullptr) {
				for (int i = 0; i < mobsList->count(); i++) {
					IWorldEntity* mobEntity = mobsList->getDirect(i);

					float dist = Math::dist3D(e->getPos(), mobEntity->getPos());
					if (dist <= aggroDistance) {
						if (closestMobEntity == nullptr || dist < closestMobEntityDist) {
							closestMobEntity = mobEntity;
							closestMobEntityDist = dist;
						}
					}
				}
			}

			if (closestMobEntity != nullptr) {
				aggroTarget = closestMobEntity->getWrapper();

				isAggroTargetChanged = true;
				//GlobalAccessLogHandler::devLogHandler->post_debug("New aggro target");
			}
		} else {
			// Check if player got out of aggro loss range.
			if (aggroTarget->entity == nullptr) {
				throw LogicException(LOC);
			}

			float dist = Math::dist3D(e->getPos(), aggroTarget->entity->getPos());
			if (dist > aggroLossDistance) {
				aggroTarget = nullptr;

				isAggroTargetChanged = true;
				//GlobalAccessLogHandler::devLogHandler->post_debug("Aggro target Sout of range");
			}
		}

		if (isAggroTargetChanged) {
			// Do an aim even if aggro target is nullptr.
			customCubesCannonTurret->addAction_ActionSetAimTarget(aggroTarget != nullptr ? aggroTarget->entity : nullptr);
		}

		// Also wait here for aggro target to exist.
		// Aim action already active, wait for "close enough" angles.
		if (aggroTarget != nullptr && aggroTarget->entity != nullptr) {
			rpg3D::IPayloadReceiverTargetModule* attackTargetEnity_projectileTarget = aggroTarget->entity->getComponentAs< rpg3D::IPayloadReceiverTargetModule*>(true/*mustExist*/);

			Vector3 hitPos{ aggroTarget->entity->getPos() };

			if (customCubesCannonTurret->isPositionInAimAnglesRange(hitPos, 15.0f, 15.0f)) {
				return Phases::MANAGED_FIRE_PROJECTILE_AT_TARGET;
			}
		}

        break;
    }
	case Phases::MANAGED_FIRE_PROJECTILE_AT_TARGET: {
        if (!isPhaseInit) {
			if (aggroTarget == nullptr || aggroTarget->entity == nullptr) {
				return Phases::MANAGED_RELOAD;
			}

			// Do a fire projectile at target action.
			isProjectileLaunched = false;

			customCubesCannonTurret->addAction_ActionFireProjectileAtTarget(aggroTarget->entity);
		} else {
			// Wait for projectile launched event.
			if (isProjectileLaunched) {
				return Phases::POST_PROJECTILE_FIRE_COOLDOWN;
			}
		}

		//void

        break;
    }
	case Phases::POST_PROJECTILE_FIRE_COOLDOWN: {
        if (!isPhaseInit) {
            // Init a small, general cooldown.
			cooldownS = 0.4f;
		} else {
			cooldownS -= deltaS;
			if (cooldownS <= 0.0f) {
				return Phases::MANAGED_RELOAD;
			}
		}

		//void

        break;
    }
	default: {
        assert(false);

        break;
    }}

	return phase;
}

LB_Turret_CubesCannon_A::~LB_Turret_CubesCannon_A() {
	//void
}
