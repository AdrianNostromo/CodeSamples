#pragma once

#include <base/view/UIView.h>
#include <base/gh.h>
#include "IDebugView.h"

class IApp;

namespace base {
class DeV_Base : public UIView, virtual public IDebugView {priv typedef UIView super;pub dCtor(DeV_Base);
	prot IApp* app;

	pub explicit DeV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

    pub ~DeV_Base() override;
};
};
