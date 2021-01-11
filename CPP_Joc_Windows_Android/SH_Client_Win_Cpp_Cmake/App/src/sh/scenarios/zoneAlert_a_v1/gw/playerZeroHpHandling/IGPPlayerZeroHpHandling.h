#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <string>
#include <memory>

namespace zoneAlert_a_v1 {
class IGPPlayerZeroHpHandling : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual void setData(
		std::shared_ptr<std::string> playerDestroyedTeleportZoneName, std::shared_ptr<std::string> playerDestroyedTeleportPlayerPosEntityId,
		bool playerDestroyed_restartScenario, bool playerDestroyed_restartZoneIfActive) = 0;

    pub ~IGPPlayerZeroHpHandling() override = default;
};
};
