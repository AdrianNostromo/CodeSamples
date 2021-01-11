#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGPPlayerZeroHpHandling.h"

namespace base {
	class GWEvent;
	class EntityEvent;
};

namespace zoneAlert_a_v1 {
class GPPlayerZeroHpHandling final : public GamePart, virtual public IGPPlayerZeroHpHandling {priv typedef GamePart super;pub dCtor(GPPlayerZeroHpHandling);
	priv ArrayList<std::shared_ptr<IEventListener>> playerEventsListenersList{};
	
	priv std::shared_ptr<std::string> playerDestroyedTeleportZoneName = nullptr;
	priv std::shared_ptr<std::string> playerDestroyedTeleportPlayerPosEntityId = nullptr;
	priv bool playerDestroyed_restartScenario = false;
	priv bool playerDestroyed_restartZoneIfActive = false;

	pub explicit GPPlayerZeroHpHandling(base::IGameWorld* gw);
	
	pub void setData(
		std::shared_ptr<std::string> playerDestroyedTeleportZoneName, std::shared_ptr<std::string> playerDestroyedTeleportPlayerPosEntityId,
		bool playerDestroyed_restartScenario, bool playerDestroyed_restartZoneIfActive) final;

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	priv void onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event);

    pub ~GPPlayerZeroHpHandling() override;
};
};
