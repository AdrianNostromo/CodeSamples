#include "TEE_Base.h"
#include "config/ToolConfigEntityEditor.h"
#include <base/math/Math.h>
#include <base/assets/IAssetsManager.h>

using namespace rpg3D;

int TEE_Base::ID = GetNew_ID();

int TEE_Base::getId() {
	return ID;
}

TEE_Base::TEE_Base(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData)
	: super(config),
	config(config),
	extraData(extraData)
{
	//void
}

void TEE_Base::create(IAssetsManager* assetsManager) {
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

void TEE_Base::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		if (!isInitExtras) {
			throw LogicException(LOC);
		}
	}
}

bool TEE_Base::canTrigger() {
	if (triggerCooldownS > 0 || getIsOperatingAny()) {
		return false;
	}

	return true;
}

void TEE_Base::chargeCooldown() {
	triggerCooldownS = config->cooldownS;

	onCooldownStateChanged();
}

void TEE_Base::onCooldownStateChanged() {
	//void
}

void TEE_Base::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (triggerCooldownS > 0) {
		triggerCooldownS = Math::max(0.0f, triggerCooldownS - deltaS);

		if (triggerCooldownS <= 0) {
			onCooldownStateChanged();
		}
	}
}

TEE_Base::~TEE_Base() {
	//void
}
