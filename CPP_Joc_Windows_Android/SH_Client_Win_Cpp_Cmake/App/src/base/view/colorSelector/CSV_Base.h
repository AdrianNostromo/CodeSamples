#pragma once

#include <base/view/UIView.h>
#include "IColorSelectorView.h"

namespace base {
class CSV_Base : public UIView, virtual public IColorSelectorView {
private: typedef UIView super;
protected:
	IListener* listener = nullptr;
	std::shared_ptr<ArrayList<DynamicTextButtonConfig>> dynamicButtonConfigs = nullptr;

private:
	bool isHistoryActive = false;
	bool isHistoryExists = false;

	bool isAlphaEditable = true;
	int alphaOverrideHexValue;

	Color activeColor{ "#ffffffff" };

	Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* userDataMap = nullptr;

public:
    CSV_Base(CSV_Base const&) = delete;
    CSV_Base(CSV_Base &&) = default;
    CSV_Base& operator=(CSV_Base const&) = delete;
    CSV_Base& operator=(CSV_Base &&) = default;

    explicit CSV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* getUserDataMap() override;

	using super::show;
	void show(bool doAnimation, IAnimationListener* menuAnimationListener, Color* initialColor, std::shared_ptr<ArrayList<DynamicTextButtonConfig>> dynamicButtonConfigs, bool isAlphaEditable, int alphaOverrideHexValue) override;

	pub void setListener(IListener* listener) final;
	pub void removeListener(IAutoListener* listener) override;

	std::string computeStateFlags(std::string baseState) override;

	Color* getActiveColor();
	virtual void setActiveColor(Color& activeColor, bool updateSliders, bool animateSliders);

    ~CSV_Base() override;
protected:
	bool getIsHistoryActive();
	void setIsHistoryActive(bool isHistoryActive, bool updateShowState);
	bool getIsAlphaEditable();
	int getAlphaOverrideHexValue();
	virtual void setIsAlphaEditable(bool isAlphaEditable, int alphaOverrideHexValue, bool updateShowState);
	bool getIsHistoryExists();
	void setIsHistoryExists(bool isHistoryExists, bool updateShowState);

};
};
