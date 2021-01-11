#pragma once

#include "CSV_HistoryList.h"
#include <base/interaction/CbTouchListener.h>

namespace base {
class ColorSelectorView : public CSV_HistoryList {
private: typedef CSV_HistoryList super;
private:
	static ArrayList<MenuItemConfig*>* viewItemConfigs;
	static MenuItemConfig* new_menuItemConfigs_contentArea();

	static ArrayList<StateChangeDurations*>* viewAnimationDurations;

	CbTouchListener touchListener{};

	IInteractiveDrawable2D* touchAreaFullScreen = nullptr;
	IInteractiveDrawable2D* contentArea_touchAreaContentBg = nullptr;

public:
    ColorSelectorView(ColorSelectorView const&) = delete;
    ColorSelectorView(ColorSelectorView &&) = default;
    ColorSelectorView& operator=(ColorSelectorView const&) = delete;
    ColorSelectorView& operator=(ColorSelectorView &&) = default;

    explicit ColorSelectorView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal);

    ~ColorSelectorView() override;
protected:
	void onSelectedStateChanged() override;
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

};
};
