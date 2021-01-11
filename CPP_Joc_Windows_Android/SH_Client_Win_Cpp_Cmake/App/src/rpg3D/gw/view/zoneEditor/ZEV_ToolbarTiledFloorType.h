#pragma once

#include <base/gh.h>
#include "ZEV_Base.h"

namespace rpg3D {
class ZEV_ToolbarTiledFloorType : public ZEV_Base {priv typedef ZEV_Base super;pub dCtor(ZEV_ToolbarTiledFloorType);
    pub explicit ZEV_ToolbarTiledFloorType(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

    pub ~ZEV_ToolbarTiledFloorType() override;
};
};
