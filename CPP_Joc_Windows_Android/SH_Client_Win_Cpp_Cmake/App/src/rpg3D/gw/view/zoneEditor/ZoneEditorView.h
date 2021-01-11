#pragma once

#include <base/gh.h>
#include "ZEV_KeyListeners.h"
#include "IZoneEditorView.h"

namespace rpg3D {
class ZoneEditorView : public ZEV_KeyListeners, public virtual IZoneEditorView {priv typedef ZEV_KeyListeners super;pub dCtor(ZoneEditorView);
    priv static ArrayList<MenuItemConfig*>* viewItemConfigs;

	priv static ArrayList<StateChangeDurations*>* viewAnimationDurations;

	pub explicit ZoneEditorView(
		IApp* app,
		IAppUtils* appUtils);

	pub using super::show;
	pub void show(
		bool doAnimation, ViewDataBase* viewData, IAnimationListener* menuAnimationListener,
		base::IBlockingInputLayer* blockingStreamLayer);

    pub ~ZoneEditorView() override;
};
};
