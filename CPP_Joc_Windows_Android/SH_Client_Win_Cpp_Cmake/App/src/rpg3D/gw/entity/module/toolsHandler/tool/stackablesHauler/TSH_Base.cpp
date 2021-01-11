#include "TSH_Base.h"
#include <base/math/Math.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/stackablesHauler/config/ToolConfigStackablesHauler.h>
#include <base/assets/IAssetsManager.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/capabilities/ICapabilities.h>

using namespace rpg3D;

int TSH_Base::ID = GetNew_ID();

int TSH_Base::getId() {
	return ID;
}

TSH_Base::TSH_Base(ToolConfigStackablesHauler* config)
	: super(config),
	config(config)
{
	initGWCapabilityRequirementBitIndex(worldGame3D::gw::ICapabilities::STACKABLES_HAULER_BIT_INDEX);
}

void TSH_Base::create(IAssetsManager* assetsManager) {
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

bool TSH_Base::canTrigger() {
	if (triggerCooldownS > 0 || getIsOperatingAny()) {
		return false;
	}

	return true;
}

void TSH_Base::chargeCooldown() {
	triggerCooldownS = config->cooldownS;

	onCooldownStateChanged();
}

void TSH_Base::onCooldownStateChanged() {
	//void
}

void TSH_Base::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (triggerCooldownS > 0) {
		triggerCooldownS = Math::max(0.0f, triggerCooldownS - deltaS);

		if (triggerCooldownS <= 0) {
			onCooldownStateChanged();
		}
	}
}

TSH_Base::~TSH_Base() {
	//void
}
