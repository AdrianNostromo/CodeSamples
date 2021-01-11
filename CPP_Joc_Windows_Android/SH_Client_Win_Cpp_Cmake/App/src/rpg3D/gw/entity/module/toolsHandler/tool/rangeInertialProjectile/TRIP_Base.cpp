#include <base/math/Math.h>
#include "TRIP_Base.h"
#include <base/exceptions/LogicException.h>
#include <base/assets/IAssetsManager.h>

using namespace rpg3D;

int TRIP_Base::ID = GetNew_ID();

int TRIP_Base::getId() {
	return ID;
}

TRIP_Base::TRIP_Base(ToolConfigRangeInertialProjectile* config, std::shared_ptr<ExtraData> extraData)
    : super(config),
      config(config),
      extraData(extraData)
{
    //void
}

void TRIP_Base::create(IAssetsManager* assetsManager) {
    super::create(assetsManager);

	if (config->activationSoundConfig_id != nullptr) {
		activationSound = assetsManager->getSound(*config->activationSoundConfig_id);
		if (activationSound == nullptr) {
			throw LogicException(LOC);
		}
	} else {
		activationSound = nullptr;
	}
}

ToolConfigRangeInertialProjectile* TRIP_Base::getLocalConfig() {
    return config->castAs<ToolConfigRangeInertialProjectile*>();
}

TRIP_Base::~TRIP_Base() {
    //void
}
