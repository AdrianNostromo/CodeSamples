#pragma once

#include <base/view/UIView.h>
#include "IListMenuCardPopupView.h"

namespace base {
class LMCPV_Base : public UIView, virtual public IListMenuCardPopupView {
private: typedef UIView super;
protected:
	std::shared_ptr<Config> viewConfig = nullptr;
	IListener* listener = nullptr;

public:
    LMCPV_Base(LMCPV_Base const&) = delete;
    LMCPV_Base(LMCPV_Base &&) = default;
    LMCPV_Base& operator=(LMCPV_Base const&) = delete;
    LMCPV_Base& operator=(LMCPV_Base &&) = default;

    explicit LMCPV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	using super::show;
	void show(bool doAnimation, IAnimationListener* menuAnimationListener, std::shared_ptr<Config> viewConfig, IListener* listener) override;

	std::string computeStateFlags(std::string baseState) override;

    ~LMCPV_Base() override;
protected:
	void onSelectedStateChanged() override;

	void disposeMain() override;
private:
	void localAnimationListener_onAnimationFinished(IGenericDrawable* target);

	void helperDisposeRequest();

};
};
