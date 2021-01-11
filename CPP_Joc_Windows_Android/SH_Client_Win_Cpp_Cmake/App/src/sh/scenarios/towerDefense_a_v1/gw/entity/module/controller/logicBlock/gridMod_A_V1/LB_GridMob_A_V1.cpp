#include <base/math/Math.h>
#include "LB_GridMob_A_V1.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/template/statusEffects/StatusEffectsTemplate.h>
#include <rpg3D/gw/entity/module/statusEffects/IStatusEffectsModule.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectStun.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/event/listener/IEventListener.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::mobUnit;

const float LB_GridMob_A_V1::AggroTargetMoveRange = 0.15f;

LB_GridMob_A_V1::LB_GridMob_A_V1()
	: super(0.1f/*tickCooldownS*/)
{
	//void
}

void LB_GridMob_A_V1::onEntityChanged() {
	super::onEntityChanged();

	if (e == nullptr) {
		//void
	} else {
		//void
	}
}

void LB_GridMob_A_V1::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		// Events with autoListenersList_whileActive are automatically cleaned up in this case.

		//void
	} else {
		e->addEventListener(
			rpg3D::IStatusEffectsModule::EVENT_statusEffects_changed->type,
			std::bind(&LB_GridMob_A_V1::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList_whileActive
		);
	}
}

void LB_GridMob_A_V1::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IStatusEffectsModule::EVENT_statusEffects_changed->type) {
		isDirty_isEntityStunned = true;
	}else {
		throw LogicException(LOC);
	}
}

void LB_GridMob_A_V1::logicTick(float deltaS) {
	super::logicTick(deltaS);

	//asdA40;
	//if(isDirty_isEntityStunned) {
	//	isDirty_isEntityStunned = false;

	//	isEntityStunned = statusEffects->getIsEffectActive(rpg3D::CfgStatusEffectStun::TYPE);
	//}
	//if(isEntityStunned) {
	//	isLogicResultDirty = true;

	//	// A.I. is disabled while this entity is stunned.
	//	return ;
	//}

	//if(aggroTarget != nullptr && aggroTarget->entity == nullptr) {
	//	aggroTarget = nullptr;
	//}

 //	if(aggroTarget == nullptr) {
	//	// Check if player is in aggro range.
	//	IWorldEntity* playerEntity = gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/);

	//	if(playerEntity != nullptr) {
	//		float dist = Math::dist3D(e->getPos(), playerEntity->getPos());
	//		if (dist <= aggroDistance) {
	//			aggroTarget = playerEntity->getWrapper();

	//			isLogicResultDirty = true;
	//		}
	//	}
	//}else {
	//	// Check if player got out of aggro loss range.
	//	if(aggroTarget->entity == nullptr) {
	//		throw LogicException(LOC);
	//	}

	//	float dist = Math::dist3D(e->getPos(), aggroTarget->entity->getPos());
	//	if(dist > aggroLossDistance) {
	//		//stop
	//		movingEntity->addAction_stopMovement(true/*stopPhysicsLinearVelocitiesAlso*/);

	//		aggroTarget = nullptr;
	//	}
	//}

 //	if(isLogicResultDirty) {
	//    isLogicResultDirty = false;

	//    if(aggroTarget != nullptr) {
	//	    // Move towards target.
	//	    Vector2 p{};
	//	    movingEntity->addAction_ActionToTargetMovementContinuous(
	//		    IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly,
	//		    p, aggroTarget,
	//		    AggroTargetMoveRange, false,
	//		    true, true
	//	    );
	//    }
 //	}
}

LB_GridMob_A_V1::~LB_GridMob_A_V1() {
	//void
}
