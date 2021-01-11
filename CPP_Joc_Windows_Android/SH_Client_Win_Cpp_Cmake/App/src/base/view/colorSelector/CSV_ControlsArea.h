#pragma once

#include "CSV_PreviewArea.h"
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <base/interaction/CbTouchListener.h>
#include <graphics/visual2d/drawable/ISlider2D.h>

namespace base {
class CSV_ControlsArea : public CSV_PreviewArea {
private: typedef CSV_PreviewArea super;
protected:
	static MenuItemConfig* new_menuItemConfigs_contentArea_controlsArea();

private:
	CbTouchListener touchListener{};

	base::ISlider2D::CbListener sliderEventListener{};

	base::ISlider2D* colorSliderR = nullptr;
	base::ISlider2D* colorSliderG = nullptr;
	base::ISlider2D* colorSliderB = nullptr;
	base::ISlider2D* colorSliderA = nullptr;
	
	base::BitmapLabel* lblSliderValueR = nullptr;
	base::BitmapLabel* lblSliderValueG = nullptr;
	base::BitmapLabel* lblSliderValueB = nullptr;
	base::BitmapLabel* lblSliderValueA = nullptr;
	
	IInteractiveDrawable2D* taBtnA = nullptr;
	IInteractiveDrawable2D* taBtnB = nullptr;
	IInteractiveDrawable2D* taBtnC = nullptr;
	
	base::BitmapLabel* lblBtnA = nullptr;
	base::BitmapLabel* lblBtnB = nullptr;
	base::BitmapLabel* lblBtnC = nullptr;

	bool isDirty_controls = false;
	
public:
    CSV_ControlsArea(CSV_ControlsArea const&) = delete;
    CSV_ControlsArea(CSV_ControlsArea &&) = default;
    CSV_ControlsArea& operator=(CSV_ControlsArea const&) = delete;
    CSV_ControlsArea& operator=(CSV_ControlsArea &&) = default;

    explicit CSV_ControlsArea(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	virtual void setActiveColor(Color& activeColor, bool updateSliders, bool animateSliders) override;

	void visualLoop(float deltaS, int deltaMS) override;

	void updateControlLblText(int controlIndex, std::string newText) override;

    ~CSV_ControlsArea() override;
protected:
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	void setIsAlphaEditable(bool isAlphaEditable, int alphaOverrideHexValue, bool updateShowState) override;

	void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;
	void onSelectedStateChanged() override;

private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	void slider_onValueChanged(ISlider2D* target);

	base::BitmapLabel* getControlLbl(int controlIndex);

	void syncDynamicContent();
	float syncDynamicBtn(DynamicTextButtonConfig* dynamicButtonConfig, IInteractiveDrawable2D* taBtn, base::BitmapLabel* lblBtn, float rightWallPosX);
	void handleTouchRequest_dynamicButtonActivated(DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor);

};
};
