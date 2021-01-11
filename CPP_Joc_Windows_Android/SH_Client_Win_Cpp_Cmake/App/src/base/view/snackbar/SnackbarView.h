#pragma once

#include <base/gh.h>
#include "SV_Controls.h"
#include <base/interaction/CbTouchListener.h>

class ISprite2D;
namespace base {
	class ISprite9Slice2D;
};

namespace base {
namespace snackbar {
class SnackbarView : public SV_Controls {priv typedef SV_Controls super;pub dCtor(SnackbarView);
	priv static ArrayList<MenuItemConfig*>* viewItemConfigs;
	priv static MenuItemConfig* new_menuItemConfigs_contentArea();

	priv static ArrayList<StateChangeDurations*>* viewAnimationDurations;

	priv CbTouchListener touchListener{};

	priv IInteractiveDrawable2D* contentArea_touchAreaContentBg = nullptr;
	priv ISprite2D* contentArea_bg = nullptr;
	priv base::ISprite9Slice2D* bodyOutterShadow = nullptr;

    pub explicit SnackbarView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler);

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;
	prot void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

	prot void onSelectedStateChanged() override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	pub ~SnackbarView() override;
};
};
};
