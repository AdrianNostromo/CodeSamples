#pragma once

#include <base/gh.h>
#include "TEEPa_Base.h"
#include <base/math/Vector3Int.h>

namespace rpg3D {
class TEEPa_Tool : public TEEPa_Base {priv typedef TEEPa_Base super;pub dCtor(TEEPa_Tool);
	priv static const int TouchMode_TOOL;

	priv std::shared_ptr<Vector3Int> lastDrawnVoxel = nullptr;

    pub explicit TEEPa_Tool(ToolConfigEEPainter* config, std::shared_ptr<ExtraData> extraData);

	prot int computeTouchModeUnderPoint(float screenX, float screenY) override;

	prot void onIsOperatingChanged(bool isOperating) override;

	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		Vector2& screenPos,
		Vector2& currentDragDeltaSensitised) override;

    pub ~TEEPa_Tool() override;
};
};
