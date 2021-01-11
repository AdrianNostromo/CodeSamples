#pragma once

#include "LMCPV_List.h"
#include <base/interaction/CbTouchListener.h>

namespace base {
class ListMenuCardPopupView : public LMCPV_List {
private: typedef LMCPV_List super;
private:
	static ArrayList<MenuItemConfig*>* viewItemConfigs;

	static ArrayList<StateChangeDurations*>* viewAnimationDurations;

	CbTouchListener touchListener{};

	IInteractiveDrawable2D* touchAreaFullScreen = nullptr;

public:
    ListMenuCardPopupView(ListMenuCardPopupView const&) = delete;
    ListMenuCardPopupView(ListMenuCardPopupView &&) = default;
    ListMenuCardPopupView& operator=(ListMenuCardPopupView const&) = delete;
    ListMenuCardPopupView& operator=(ListMenuCardPopupView &&) = default;

    explicit ListMenuCardPopupView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal);

    ~ListMenuCardPopupView() override;
protected:
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	void onSelectedStateChanged() override;

	void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	void helperCallCloseRequestedEvent();

};
};
