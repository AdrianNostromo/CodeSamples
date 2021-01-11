#pragma once

#include "DV_Controls.h"
#include <base/interaction/CbTouchListener.h>

namespace base {
class DialogView : public DV_Controls {
private: typedef DV_Controls super;
private:
	static ArrayList<MenuItemConfig*>* viewItemConfigs;
	static MenuItemConfig* new_menuItemConfigs_contentArea();

	static ArrayList<StateChangeDurations*>* viewAnimationDurations;

	CbTouchListener touchListener{};

	IInteractiveDrawable2D* touchAreaFullScreen = nullptr;
	IInteractiveDrawable2D* contentArea_touchAreaContentBg = nullptr;

public:
    DialogView(DialogView const&) = delete;
    DialogView(DialogView &&) = default;
    DialogView& operator=(DialogView const&) = delete;
    DialogView& operator=(DialogView &&) = default;

    explicit DialogView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal);

    ~DialogView() override;
protected:
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;
	void onSelectedStateChanged() override;

private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
};
};
