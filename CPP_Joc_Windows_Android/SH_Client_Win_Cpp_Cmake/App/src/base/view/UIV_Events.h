#pragma once

#include <base/gh.h>
#include "UIV_Base.h"
#include "listener/IDispatcherAutoListener.h"

namespace base {
class UIV_Events : public UIV_Base, virtual public IDispatcherAutoListener {priv typedef UIV_Base super;pub dCtor(UIV_Events);
    prot base::TypedEventDispatcher1D<ViewEvent> events{};

    pub explicit UIV_Events(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

    pub base::TypedEventDispatcher1D<ViewEvent>& getEvents() final;

	prot void disposeMain() override;
    pub ~UIV_Events() override;
};
};
