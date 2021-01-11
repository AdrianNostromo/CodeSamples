#include "TZE_Base.h"
#include "config/ToolConfigZoneEditor.h"
#include <base/math/Math.h>
#include <base/assets/IAssetsManager.h>

using namespace rpg3D;

int TZE_Base::ID = GetNew_ID();

int TZE_Base::getId() {
	return ID;
}

const float TZE_Base::EDIT_RANGE = 5.0f;

TZE_Base::TZE_Base(ToolConfigZoneEditor* config, std::shared_ptr<ExtraData> extraData)
	: super(config),
	config(config),
	extraData(extraData)
{
	//void
}

void TZE_Base::create(IAssetsManager* assetsManager) {
	super::create(assetsManager);
	
	if (config->activationSoundConfig_id != nullptr) {
		activationSound = assetsManager->getSound(*config->activationSoundConfig_id);
		if (activationSound == nullptr) {
			throw LogicException(LOC);
		}
	} else {
		activationSound = nullptr;
	}

	isInitExtras = true;
}

void TZE_Base::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		if (!isInitExtras) {
			throw LogicException(LOC);
		}
	}
}

bool TZE_Base::canTrigger() {
	if (triggerCooldownS > 0 || getIsOperatingAny()) {
		return false;
	}

	return true;
}

void TZE_Base::chargeCooldown() {
	triggerCooldownS = config->cooldownS;

	onCooldownStateChanged();
}

void TZE_Base::onCooldownStateChanged() {
	//void
}

void TZE_Base::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (triggerCooldownS > 0) {
		triggerCooldownS = Math::max(0.0f, triggerCooldownS - deltaS);

		if (triggerCooldownS <= 0) {
			onCooldownStateChanged();
		}
	}
}

TZE_Base::~TZE_Base() {
	//void
}
