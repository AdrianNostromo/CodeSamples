#pragma once

#include <base/gh.h>
#include "TEE_Base.h"
#include <base/math/Vector2.h>

namespace rpg3D {
	class IVoxelizedEditorM_Edit;
};

namespace rpg3D {
class TEE_ToolActivation : public TEE_Base {priv typedef TEE_Base super;pub dCtor(TEE_ToolActivation);
	prot static int getNew_touchModeIncrementor();
	prot static const int TouchMode_IDDLE;

	priv bool usesAutoToolModeActivationSound;

	prot IVoxelizedEditorM_Edit* entityEditor = nullptr;

	// This is used in case a drag is instantly ended the same frame. This ensures it is processed.
	priv bool isTouchDownPulse = false;
	priv bool isTouchDragStopRequested = false;
	priv bool isDragActive = false;
	// Note. This is also sensitised for locked_mouse mode, but is normal for normal_mouse mode.
	priv Vector2 currentDrag_screenPos{};
	priv Vector2 currentDrag_deltaSensitised{};

	priv int touchMode = TouchMode_IDDLE;

	pub explicit TEE_ToolActivation(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData,
		bool usesAutoToolModeActivationSound);

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

	prot int getTouchMode();
	prot void stopCurrentTouchMode();
	priv void setTouchMode(int touchMode);
	prot virtual void onTouchModeChange(int touchMode, int oldTouchMode);

	prot virtual int computeTouchModeUnderPoint(float screenX, float screenY);

	prot void toolFilterStart(Vector2& screenPos);
	prot void toolFilterUpdate(Vector2& screenPos, Vector2& sensitisedScreenCursorMoveDelta);
	prot void toolFilterEnd(int cursorIndex);

	prot virtual void tickActiveTouchMode(
		int touchMode, float deltaS,
		Vector2& screenPos,
		Vector2& currentDragDeltaSensitised);

	prot void onEntityChanged() override;
	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

    pub ~TEE_ToolActivation() override;
};
};
