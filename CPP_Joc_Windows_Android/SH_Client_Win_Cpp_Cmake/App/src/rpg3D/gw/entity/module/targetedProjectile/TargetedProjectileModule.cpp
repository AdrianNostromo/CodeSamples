#include "TargetedProjectileModule.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/targetedProjectile/TargetedProjectileTemplate.h>
#include <rpg3D/gw/entity/template/payloadReceiverTarget/PayloadReceiverTargetTemplate.h>
#include <rpg3D/gw/entity/module/payloadReceiverTarget/IPayloadReceiverTargetModule.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/module/statusEffects/StatusEffectsModule.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>
#include <rpg3D/gw/entity/module/flying/IFlyingModule.h>

using namespace rpg3D;

TargetedProjectileModule::TargetedProjectileModule(
    IWorldEntity* entity,
    TargetedProjectileTemplate* t)
    : super(entity, t)
{
	e->addEventListener(
		rpg3D::IFlyingModule::EVENT_FlyingFinished_TargetLost->type,
		std::bind(&TargetedProjectileModule::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&TargetedProjectileModule::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void TargetedProjectileModule::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IFlyingModule::EVENT_FlyingFinished_TargetLost->type) {
		e->markEntityForRemoval(ENT_REM_REASON_PURPOSE_FINISHED, false);
	} else {
		throw LogicException(LOC);
	}
}

void TargetedProjectileModule::initTargetEntity(
	IWorldEntity* targetEntity)
{
	this->targetEntity = targetEntity->getWrapper();

	this->targetEntity_projectileTarget = targetEntity->getComponentAs< rpg3D::IPayloadReceiverTargetModule*>(true/*mustExist*/);
}

void TargetedProjectileModule::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
	if (isPayloadTransferred) {
		throw LogicException(LOC);
	}
	if (targetEntity != nullptr && targetEntity->entity == nullptr) {
		// Target entity removed, wait until flying finishes event to the last known targetEntity pos.
		return;
	}

	// asdB; Implement when needed. Use same logic as projectiles to detect impacts but ghost_clip through walls also and add a impact_entity_test for exact_entity_check.
	throw LogicException(LOC);

	//asdB;
	//// Check dist to target.
	//Vector3 bakedDestPos{ targetEntity->entity->getPos() };
	//bakedDestPos.add(targetEntityGlobalOffset);
	//float distToTarget = e->getPos()->distTo(
	//	bakedDestPos
	//);

	//if (distToTarget <= impactDetectionDist) {
	//	for (int i = 0; i < payloadsList.count(); i++) {
	//		std::shared_ptr<rpg3D::EffectPayload>& payloadEntry = payloadsList.getReference(i);

	//		payloadEntry->transferToEntity(entity, targetEntity->entity);
	//	}

	//	isPayloadTransferred = true;

	//	e->markEntityForRemoval(ENT_REM_REASON_PURPOSE_FINISHED, false);
	//}
}

TargetedProjectileModule::~TargetedProjectileModule() {
	//void
}

