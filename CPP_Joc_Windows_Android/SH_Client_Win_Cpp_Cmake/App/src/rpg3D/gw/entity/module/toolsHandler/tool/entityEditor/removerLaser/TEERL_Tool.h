#pragma once

#include <base/gh.h>
#include "TEERL_Base.h"
#include <base/view/dialog/listener/CbAutoListenerDialogView.h>

namespace rpg3D {
class TEERL_Tool : public TEERL_Base {priv typedef TEERL_Base super;pub dCtor(TEERL_Tool);
    prot static const int TouchMode_TOOL;

	priv int voxelsRemovedOnCurrentDrag = 0;

	priv base::IDialogView* dialogView = nullptr;
	priv base::CbAutoListenerDialogView dialogEventListener{};

	pub explicit TEERL_Tool(ToolConfigEERemoverLaser* config, std::shared_ptr<ExtraData> extraData);

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
    pub ~TEERL_Tool() override;
};
};
