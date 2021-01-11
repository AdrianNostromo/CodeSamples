#pragma once

#include <base/gh.h>
#include "EVM_History.h"

namespace base {
	class GWEvent;
};

namespace rpg3D {
class EVM_RotationResetter : public EVM_History {priv typedef EVM_History super;pub dCtor(EVM_RotationResetter);
    // If < 1f; Reset is in progress.
	priv float resetRotationCP = 1.0f;

	priv Quaternion startRot{};
	priv Quaternion endRot{};

	priv Quaternion tmp_rot{};
	
	pub explicit EVM_RotationResetter(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

	pub void resetEditEntityRotation() final;

	prot void onIsEditingEnabledChanged() override;

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

    pub ~EVM_RotationResetter() override;
};
};
