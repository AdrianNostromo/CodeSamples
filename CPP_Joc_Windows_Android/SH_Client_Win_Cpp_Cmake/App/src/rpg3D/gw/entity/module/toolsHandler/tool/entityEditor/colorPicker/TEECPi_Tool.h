#pragma once

#include <base/gh.h>
#include "TEECPi_Base.h"

namespace rpg3D {
class TEECPi_Tool : public TEECPi_Base {priv typedef TEECPi_Base super;pub dCtor(TEECPi_Tool);
	prot static const int TouchMode_TOOL;

	priv bool isInitialTouchedVoxel = false;
	priv int lastTouched_registeredVertexColorIndex;

	pub explicit TEECPi_Tool(ToolConfigEEColorPicker* config, std::shared_ptr<ExtraData> extraData);

	prot int computeTouchModeUnderPoint(float screenX, float screenY) override;

	prot void onIsOperatingChanged(bool isOperating) override;

	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		Vector2& screenPos,
		Vector2& currentDragDeltaSensitised) override;

    pub ~TEECPi_Tool() override;
};
};
