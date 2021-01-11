#pragma once

#include "GPManBase.h"

namespace rpg3D {
class GPManPlatforms : public GPManBase {priv typedef GPManBase super;pub dCtor(GPManPlatforms);
	// These platforms are ignored at the start because the player can spawn on a platform and it should not trigger then.
	// This list is cleared after ~2_loop_ticks.
	// This contains platforms instance ids.
	pub ArrayList<std::shared_ptr<std::string>> playerSpawnIgnoredPlatformsActivation{};
	pub int framesToIgnorePlatformActivations = 0;

	priv bool isPlatformsActivationEnabled = true;

	pub explicit GPManPlatforms(base::IGameWorld* gw);

	pub void addPlayerSpawnIgnoredPlatformsActivation(std::shared_ptr<std::string> platformInstanceId) override;

	pub void setIsPlatformsActivationEnabled(bool isPlatformsActivationEnabled) final;

	priv void onGWEvent_gameLoop_post(IEventListener& eventListener, base::GWEvent& event);

	priv void onEntityEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub void disposeMain() override;
	pub ~GPManPlatforms() override;
};
};
