#pragma once

#include <base/view/UIView.h>
#include "IDialogView.h"

namespace base {
class DV_Base : public UIView, virtual public IDialogView {
private: typedef UIView super;
protected:
	std::shared_ptr<Config> dialogConfig = nullptr;
	IAutoListenerDialogView* listener = nullptr;

private:
	bool isTitleExists = false;
	bool isBodyExists = false;
	bool isControlsExists = false;

	// dataMap entries:
	// dynamic: int width
	// dynamic: int titleAreaHeight
	// dynamic: int bodyAreaHeight
	// dynamic: int controlsAreaHeight

	Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* userDataMap = nullptr;

public:
    DV_Base(DV_Base const&) = delete;
    DV_Base(DV_Base &&) = default;
    DV_Base& operator=(DV_Base const&) = delete;
    DV_Base& operator=(DV_Base &&) = default;

    explicit DV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* getUserDataMap();

	using super::show;
	void show(bool doAnimation, IAnimationListener* menuAnimationListener, std::shared_ptr<Config> config) override;

	pub void setListener(IAutoListenerDialogView* listener) final;
	pub void removeListener(IAutoListener* listener) final;

	std::string computeStateFlags(std::string baseState) override;

    ~DV_Base() override;
protected:
	virtual void initializeDialogAreas();
	
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	bool getIsTitleExists();
	void putIsTitleExists(bool isTitleExists, bool updateShowState);
	void eventChangeIsTitleExists();
	bool getIsBodyExists();
	void putIsBodyExists(bool isBodyExists, bool updateShowState);
	void eventChangeIsBodyExists();
	bool getIsControlsExists();
	void putIsControlsExists(bool isControlsExists, bool updateShowState);
	void eventChangeIsControlsExists();

};
};
