#pragma once

#include <base/gh.h>
#include "UIV_Events.h"
#include "listener/IDispatcherAutoListener.h"

namespace base {
class UIV_AutoListeners : public UIV_Events, virtual public IDispatcherAutoListener {priv typedef UIV_Events super;pub dCtor(UIV_AutoListeners);
	priv ArrayList<IAutoListener*> listenersList{};

    pub explicit UIV_AutoListeners(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

	pub virtual void addListener(IAutoListener* listener);
	pub virtual void removeListener(IAutoListener* listener);

	pub void onAutoListenerDisposing(IAutoListener* listener) final;

	prot void disposeMain() override;
    pub ~UIV_AutoListeners() override;
};
};
