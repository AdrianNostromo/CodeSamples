#include "WIM_Base.h"
#include "../../template/worldItem/WorldItemTemplate.h"
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <base/audio/sound/ISound.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>

using namespace rpg3D;

WIM_Base::WIM_Base(
	IWorldEntity* entity,
	WorldItemTemplate* t)
	: super(entity),
	t(t)
{
	//void
}

void WIM_Base::createBMain() {
	super::createBMain();

	//void
}

void WIM_Base::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	if (t->impactSoundConfig_id != nullptr) {
		collectedSound = entity->getGW()->getHandler()->getAppAssets()->getEmbeddedAssetsManager()->getSound(*t->impactSoundConfig_id);
		if (collectedSound == nullptr) {
			throw LogicException(LOC);
		}
	}
}

void WIM_Base::disconnectFromGamePartsPre() {
	//void

	super::disconnectFromGamePartsPre();
}

ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* WIM_Base::peekPayloadsList() {
    return &payloadsList;
}

void WIM_Base::addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) {
	this->payloadsList.appendReference(payload);
}

void WIM_Base::addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) {
	for(int i=0;i<payloadsList->count();i++) {
		std::shared_ptr<rpg3D::EffectPayload>& entry = payloadsList->getReference(i);

		addPayload(entry);
	}
}

void WIM_Base::onItemCollected() {
	if (collectedSound != nullptr) {
		collectedSound->play(t->impactSoundConfig_volume);
	}

	e->markEntityForRemoval(IWorldEntity::ENT_REM_REASON_PURPOSE_FINISHED, false);
}

WIM_Base::~WIM_Base() {
	//void
}
