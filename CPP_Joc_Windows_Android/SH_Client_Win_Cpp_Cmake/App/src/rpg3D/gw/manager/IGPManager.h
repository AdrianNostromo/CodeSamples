#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <worldGame3D/gw/manager/IGPManager.h>

namespace base {
	class GWEvent;
};

namespace rpg3D {
class IGPManager : virtual public base::IGPManager {
	pub virtual void addPlayerSpawnIgnoredPlatformsActivation(std::shared_ptr<std::string> platformInstanceId) = 0;

	pub virtual void setIsPlatformsActivationEnabled(bool isPlatformsActivationEnabled) = 0;

	pub static base::GWEvent* GWEvent_onPlayerActivatedPlatformNonSpawn;

    pub ~IGPManager() override = default;
};
};
