#pragma once

#include "CSV_Base.h"
#include <base/interaction/CbTouchListener.h>

namespace base {
class CSV_PreviewArea : public CSV_Base {
private: typedef CSV_Base super;
protected:
	static MenuItemConfig* new_menuItemConfigs_contentArea_previewArea();

private:
	CbTouchListener touchListener{};

	IInteractiveDrawable2D* contentArea_previewArea_btnToggleHistoryColors = nullptr;

public:
    CSV_PreviewArea(CSV_PreviewArea const&) = delete;
    CSV_PreviewArea(CSV_PreviewArea &&) = default;
    CSV_PreviewArea& operator=(CSV_PreviewArea const&) = delete;
    CSV_PreviewArea& operator=(CSV_PreviewArea &&) = default;

    explicit CSV_PreviewArea(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	void setActiveColor(Color& activeColor, bool updateSliders, bool animateSliders) override;

    ~CSV_PreviewArea() override;
protected:
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

};
};
