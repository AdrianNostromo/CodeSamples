#pragma once

#include <base/gh.h>
#include <base/view/util/ViewDataBase.h>
#include "UIV_AutoDisposable.h"

class IEventListener;
namespace base {
    class LoopEvent;
};
class UIView : public base::UIV_AutoDisposable {priv typedef base::UIV_AutoDisposable super;pub dCtor(UIView);
    prot static std::string VFLAG_in;
    prot static std::string VFLAG_out;

    prot ViewDataBase* viewData = nullptr;

    // This variable is used to make sure the app loop is called. This is used to avoid some programming errors.
    priv bool isAnyAppLoopCalled = false;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit UIView(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);
    prot void create() override;

    prot virtual void onLoopEvent_appLoop_UI(IEventListener& eventListener, base::LoopEvent& event);

    pub void show(bool doAnimation, ViewDataBase* viewData, IAnimationListener* menuAnimationListener) override;
    pub void show(bool doAnimation) override;
    pub void hide(bool doAnimation, IAnimationListener* menuAnimationListener) override;
    pub void hide(bool doAnimation) override;

    prot void onSelectedStateChanged() override;

    prot virtual void onShowRequested_pre();

    pub ~UIView() override;
};
