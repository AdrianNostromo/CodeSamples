#pragma once

#include <base/gh.h>
#include <base/view/UIView.h>

namespace base {
class SplashScreenView : public UIView {priv typedef UIView super;pub dCtor(SplashScreenView);
    priv static ArrayList<MenuItemConfig*>* viewItemConfigs;
	
    priv static ArrayList<StateChangeDurations*>* viewAnimationDurations;

    pub explicit SplashScreenView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal);
    prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

    pub ~SplashScreenView() override;
};
};
