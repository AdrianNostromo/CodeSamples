#pragma once

#include <base/gh.h>
#include "ADD_Base.h"
#include <base/math/Vector3.h>

namespace base {
	class GWEvent;
};

namespace rpg3D {
class AutoDestroyDurationModule : public ADD_Base {priv typedef ADD_Base super;pub dCtor(AutoDestroyDurationModule);
    priv float totalDistTravelled = 0.0f;
	priv float totalDurationS = 0.0f;

	priv Vector3 lastTickPos{};
	priv bool isLastTickPosInit = false;

	pub explicit AutoDestroyDurationModule(
		IWorldEntity* entity,
		AutoDestroyDurationTemplate* t);

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

    pub ~AutoDestroyDurationModule() override;
};
};
