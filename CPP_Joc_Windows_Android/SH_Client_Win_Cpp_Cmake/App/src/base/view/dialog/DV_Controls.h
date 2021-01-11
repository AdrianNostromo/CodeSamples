#pragma once

#include "DV_Body.h"
#include <base/interaction/CbTouchListener.h>

namespace base {
	class BitmapLabel;
}

namespace base {
class DV_Controls : public DV_Body {
private: typedef DV_Body super;
protected:
	static MenuItemConfig* new_menuItemConfigs_contentArea_controlsArea();

private:
	CbTouchListener touchListener{};

	ArrayList<IInteractiveDrawable2D*> cuntrolButtonTouchAreas{};
	ArrayList<base::BitmapLabel*> cuntrolButtonLbls{};

public:
    DV_Controls(DV_Controls const&) = delete;
    DV_Controls(DV_Controls &&) = default;
    DV_Controls& operator=(DV_Controls const&) = delete;
    DV_Controls& operator=(DV_Controls &&) = default;

    explicit DV_Controls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	base::BitmapLabel* getControlLbl(int controlIndex);

    ~DV_Controls() override;
protected:
	void initializeDialogAreas() override;

	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;
	void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	void dynamicButtonActivated(DynamicTextButtonConfig* dynamicButtonConfig);
	void initDynamicBtn(DynamicTextButtonConfig* dynamicButtonConfig, IInteractiveDrawable2D* taBtn, base::BitmapLabel* lblBtn);
	
	void syncDynamicButtonsPositions();

};
};
