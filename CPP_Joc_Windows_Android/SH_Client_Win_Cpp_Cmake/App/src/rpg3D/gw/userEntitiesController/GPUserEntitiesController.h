#pragma once

#include <base/gh.h>
#include "GPUEC_ToolExraActivation.h"

namespace rpg3D {
class GPUserEntitiesController : public GPUEC_ToolExraActivation {priv typedef GPUEC_ToolExraActivation super;pub dCtor(GPUserEntitiesController);
    pub explicit GPUserEntitiesController(base::IGameWorld* gw);
	prot void createBMain() override;

	priv void onGWEvent_gameLoop_localInput(IEventListener& eventListener, base::GWEvent& event);

	pub base::IGameWorld* getGW() override { return gw; };

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;

	priv void onPlayerEntityChanged();

    pub ~GPUserEntitiesController() override;
};
};
