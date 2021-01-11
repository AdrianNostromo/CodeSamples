#pragma once

#include <base/gh.h>
#include "TEEPl_Base.h"
#include <base/math/Bounds3DInt.h>
#include <base/view/dialog/listener/CbAutoListenerDialogView.h>

namespace rpg3D {
class TEEPl_Tool : public TEEPl_Base {priv typedef TEEPl_Base super;pub dCtor(TEEPl_Tool);
    prot static const int TouchMode_TOOL;

	priv static const int VOXEL_MODEL_GROUP_SIZE_LIMIT;

	priv int voxelsPlacedOnCurrentDrag = 0;

	priv Bounds3DInt tmp_gridAreaBounds{};

	priv base::IDialogView* dialogView = nullptr;
	priv base::CbAutoListenerDialogView dialogEventListener{};

	pub explicit TEEPl_Tool(ToolConfigEEPlacer* config, std::shared_ptr<ExtraData> extraData);

	priv void dialog_onDynamicButtonActivated(base::IDialogView* view, base::DynamicTextButtonConfig* dynamicButtonConfig);
	priv void dialog_onCloseRequested(base::IDialogView* view);
	priv void dialog_onContentAreaTouched(base::IDialogView* view);

	prot void onEntityChangePre() override;

	prot int computeTouchModeUnderPoint(float screenX, float screenY) override;

	prot void onIsOperatingChanged(bool isOperating) override;

	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		Vector2& screenPos,
		Vector2& currentDragDeltaSensitised) override;

	prot void disposeMain() override;
    pub ~TEEPl_Tool() override;
};
};
