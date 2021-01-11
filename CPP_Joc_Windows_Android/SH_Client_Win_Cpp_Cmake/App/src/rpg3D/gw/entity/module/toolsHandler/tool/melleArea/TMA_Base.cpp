#include <base/math/Math.h>
#include "TMA_Base.h"
#include <base/exceptions/LogicException.h>
#include <base/assets/IAssetsManager.h>

using namespace rpg3D;

int TMA_Base::ID = GetNew_ID();

int TMA_Base::getId() {
	return ID;
}

TMA_Base::TMA_Base(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData)
    : super(config),
      config(config),
      extraData(extraData)
{
    //void
}

void TMA_Base::create(IAssetsManager* assetsManager) {
	super::create(assetsManager);

	if (config->activationSoundConfig_id != nullptr) {
		activationSound = assetsManager->getSound(*config->activationSoundConfig_id);
		if (activationSound == nullptr) {
			throw LogicException(LOC);
		}
	} else {
		activationSound = nullptr;
	}

	if (config->impactSoundConfig_id != nullptr) {
		impactSound = assetsManager->getSound(*config->impactSoundConfig_id);
		if (impactSound == nullptr) {
			throw LogicException(LOC);
		}
	} else {
		impactSound = nullptr;
	}

	isInitExtras = true;
}

void TMA_Base::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		if(!isInitExtras) {
			throw LogicException(LOC);
		}
	}
}

ToolConfigMelleArea* TMA_Base::getLocalConfig() {
    return config->castAs<ToolConfigMelleArea*>();
}

ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* TMA_Base::peekPayloadsList() {
	return &payloadsList;
}

void TMA_Base::addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) {
	this->payloadsList.appendReference(payload);
}

void TMA_Base::addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) {
	for (int i = 0; i < payloadsList->count(); i++) {
		std::shared_ptr<rpg3D::EffectPayload>& entry = payloadsList->getReference(i);

		addPayload(entry);
	}
}

TMA_Base::~TMA_Base() {
    //void
}
