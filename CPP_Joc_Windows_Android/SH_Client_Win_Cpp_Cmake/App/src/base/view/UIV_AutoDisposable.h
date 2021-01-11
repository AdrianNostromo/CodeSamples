#pragma once

#include <base/gh.h>
#include "UIV_AutoListeners.h"

namespace base {
class UIV_AutoDisposable : public UIV_AutoListeners {priv typedef UIV_AutoListeners super;pub dCtor(UIV_AutoDisposable);
	prot IHandlerAutoDisposal* handlerAutoDisposal;

	priv bool isReadyForAutoDisposal = false;
    
	pub explicit UIV_AutoDisposable(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	prot void animationFinishedEvent() override;
	prot void onVisualLoopsFinished();

    pub ~UIV_AutoDisposable() override;
};
};
