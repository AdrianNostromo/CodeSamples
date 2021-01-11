#pragma once

#include <base/gh.h>
#include "ZEV_ToolbarTiledFloorType.h"

namespace rpg3D {
class ZEV_ToolbarTiledWallType : public ZEV_ToolbarTiledFloorType {priv typedef ZEV_ToolbarTiledFloorType super;pub dCtor(ZEV_ToolbarTiledWallType);
    pub explicit ZEV_ToolbarTiledWallType(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

    pub ~ZEV_ToolbarTiledWallType() override;
};
};
