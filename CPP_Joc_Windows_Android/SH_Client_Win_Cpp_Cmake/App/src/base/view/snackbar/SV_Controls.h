#pragma once

#include <base/gh.h>
#include "SV_Text.h"
#include <base/interaction/CbTouchListener.h>

class ISprite2D;
namespace base {
	class BitmapLabel;
}

namespace base {
namespace snackbar {
class SV_Controls : public SV_Text {priv typedef SV_Text super;pub dCtor(SV_Controls);
	prot static MenuItemConfig* new_menuItemConfigs_contentArea_controlsArea();

	priv CbTouchListener touchListener{};

	prot IInteractiveDrawable2D* controlTouchArea = nullptr;
	prot base::BitmapLabel* controlLbl = nullptr;
	prot ISprite2D* controlBg = nullptr;

	// This is used when the control exists and has a exclusive line.
	/// This is used to move the text above the control when the control has a long text (there is a config flag for this wide control mode).
	prot float controlsArea_exclusiveHeight = 0.0f;

	prot bool isControlExists = false;

    pub explicit SV_Controls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler);

	prot void initializeSnackbarContent() override;

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;
	prot void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	priv void dynamicButtonActivated(DynamicTextButtonConfig* dynamicButtonConfig);
	priv void initDynamicBtn(DynamicTextButtonConfig* dynamicButtonConfig, IInteractiveDrawable2D* taBtn, base::BitmapLabel* lblBtn);

	pub std::string computeStateFlags(std::string baseState) override;

	pub ~SV_Controls() override;
};
};
};
