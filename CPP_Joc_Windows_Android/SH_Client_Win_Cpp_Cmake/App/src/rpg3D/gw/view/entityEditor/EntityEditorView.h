#pragma once

#include <base/gh.h>
#include "EEV_KeyListeners.h"

namespace rpg3D {
class EntityEditorView : public EEV_KeyListeners {priv typedef EEV_KeyListeners super;pub dCtor(EntityEditorView);
	priv static ArrayList<MenuItemConfig*>* viewItemConfigs;

	priv static ArrayList<StateChangeDurations*>* viewAnimationDurations;

	pub explicit EntityEditorView(
		IApp* app, 
		IAppUtils* appUtils);

	pub using super::show;
	pub void show(
		int editorType,
		bool doAnimation, ViewDataBase* viewData, IAnimationListener* menuAnimationListener,
		base::IBlockingInputLayer* blockingStreamLayer);

	pub ~EntityEditorView() override;
};
};
