#include "TPM_Base.h"
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/module/payloadReceiverTarget/IPayloadReceiverTargetModule.h>
#include <rpg3D/gw/entity/module/statusEffects/StatusEffectsModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <rpg3D/gw/entity/template/targetedProjectile/TargetedProjectileTemplate.h>

using namespace rpg3D;

EntityRemoveReason *const TPM_Base::ENT_REM_REASON_PURPOSE_FINISHED = new EntityRemoveReason("ENT_REM_REASON_PURPOSE_FINISHED", true, true);

TPM_Base::TPM_Base(
	IWorldEntity* entity,
	TargetedProjectileTemplate* t)
	: super(entity),
	  t(t)
{
	//void
}

ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* TPM_Base::peekPayloadsList() {
    return &payloadsList;
}

void TPM_Base::addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) {
	this->payloadsList.appendReference(payload);
}

void TPM_Base::addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) {
	for(int i=0;i<payloadsList->count();i++) {
		std::shared_ptr<rpg3D::EffectPayload>& entry = payloadsList->getReference(i);

		addPayload(entry);
	}
}

void TPM_Base::createBMain() {
    super::createBMain();

	//void
	
}

void TPM_Base::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	if (t->impactSoundConfig_id != nullptr) {
		impactSound = entity->getGW()->getHandler()->getAppAssets()->getEmbeddedAssetsManager()->getSound(*t->impactSoundConfig_id);
		if (impactSound == nullptr) {
			throw LogicException(LOC);
		}
	}
}

void TPM_Base::disconnectFromGamePartsPre() {
	//void

	super::disconnectFromGamePartsPre();
}

TPM_Base::~TPM_Base() {
	//void
}
