#pragma once

#include <base/gh.h>
#include "TEE_InteractionFilters.h"

namespace rpg3D {
	class VoxelizedEditorTemplate;
	class IVoxelizedEditorM_Edit;
};

namespace rpg3D {
class TEE_Rotate : public TEE_InteractionFilters {priv typedef TEE_InteractionFilters super;pub dCtor(TEE_Rotate);
    priv static const float dragSpeedMultiplier;

	priv static const int TouchMode_ROTATE;

	// Skip the first rotation tick because there is no touch movement on it.
	priv bool isFirstRotationTickSkiped = false;

	pub explicit TEE_Rotate(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData,
		bool usesAutoToolModeActivationSound);

	prot int computeTouchModeUnderPoint(float screenX, float screenY) override;
	prot void onTouchModeChange(int touchMode, int oldTouchMode) override;
	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		Vector2& screenPos,
		Vector2& currentDragDeltaSensitised) override;

    pub ~TEE_Rotate() override;
};
};
