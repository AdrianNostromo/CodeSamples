#pragma once

#include <base/gh.h>
#include "TZE_Base.h"
#include <base/math/Vector2.h>
#include <base/memory/SharedPointer.h>

namespace rpg3D {
	class ToolActivationInfo_Base;
	class IZoneEditorM_Edit;
};

namespace rpg3D {
class TZE_ToolActivation : public TZE_Base {priv typedef TZE_Base super;pub dCtor(TZE_ToolActivation);
	prot static int getNew_touchModeIncrementor();
	prot static const int TouchMode_IDDLE;

	priv bool usesAutoToolModeActivationSound;

	prot rpg3D::IZoneEditorM_Edit* entityEditor = nullptr;

	// This is used in case a drag is instantly ended the same frame. This ensures it is processed.
	priv bool isTouchDownPulse = false;
	priv bool isTouchDragStopRequested = false;
	priv bool isDragActive = false;
	// Note. This is also sensitised for locked_mouse mode, but is normal for normal_mouse mode.
	priv sp<ToolActivationInfo_Base> currentDrag_touchData = nullptr;

	priv int touchMode = TouchMode_IDDLE;

	pub explicit TZE_ToolActivation(ToolConfigZoneEditor* config, std::shared_ptr<ExtraData> extraData,
		bool usesAutoToolModeActivationSound);

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

	prot int getTouchMode();
	prot void stopCurrentTouchMode();
	priv void setTouchMode(int touchMode);
	prot virtual void onTouchModeChange(int touchMode, int oldTouchMode);

	prot virtual int computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData);

	prot void toolFilterStart(sp<ToolActivationInfo_Base> touchData);
	prot void toolFilterUpdate(sp<ToolActivationInfo_Base> touchData);
	prot void toolFilterEnd(sp<ToolActivationInfo_Base> touchData);

	prot virtual void tickActiveTouchMode(
		int touchMode, float deltaS,
		ToolActivationInfo_Base* touchData);

	prot void onEntityChanged() override;
	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

    pub ~TZE_ToolActivation() override;
};
};
