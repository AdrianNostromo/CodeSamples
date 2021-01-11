#pragma once

#include <base/gh.h>
#include "LD_Listeners.h"

namespace base {
class LD_Dispatcher : public LD_Listeners {priv typedef LD_Listeners super;pub dCtor(LD_Dispatcher);
    // Must be float because it is used in float math (To avoid unwanted rounding).
	// -1: max fps available.
	priv static const float INPUT_DISPATCH_FPS;
	priv static const float GENERAL_FPS;
	priv static const float ECO_FPS;
	priv static const float GAME_WORLD_FPS;
	priv static const float UI_FPS;

	priv float inputDispatch_deltaS = 0.0f;
	priv int inputDispatch_deltaMS = 0;
	priv float inputDispatch_appTimeS = 0.0f;

	priv float general_deltaS = 0.0f;
	priv int general_deltaMS = 0;
	priv float general_appTimeS = 0;

	priv float eco_deltaS = 0.0f;
	priv int eco_deltaMS = 0;
	priv float eco_appTimeS = 0;

	priv float gameWorld_deltaS = 0.0f;
	priv int gameWorld_deltaMS = 0;
	priv float gameWorld_appTimeS = 0;

	priv float uI_deltaS = 0.0f;
	priv int uI_deltaMS = 0;
	priv float uI_appTimeS = 0;

	pub explicit LD_Dispatcher();

    pub void distributeLoop(float deltaS, int deltaMS);

    pub ~LD_Dispatcher() override;
};
};
