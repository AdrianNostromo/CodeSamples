#pragma once

#include <base/gh.h>
#include "ZEV_ToolbarTiledWallType.h"
#include <base/interaction/CbTouchListener.h>

namespace base {
	class SpriteButton2D;
};

namespace rpg3D {
class ZEV_GeneralInterface : public ZEV_ToolbarTiledWallType {priv typedef ZEV_ToolbarTiledWallType super;pub dCtor(ZEV_GeneralInterface);
	prot static MenuItemConfig* new_viewItemConfig_topArea();

	priv CbTouchListener touchListener{};

	priv base::SpriteButton2D* btnBack;

    pub explicit ZEV_GeneralInterface(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	pub std::string computeStateFlags(std::string baseState) override;

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

    pub ~ZEV_GeneralInterface() override;
};
};
