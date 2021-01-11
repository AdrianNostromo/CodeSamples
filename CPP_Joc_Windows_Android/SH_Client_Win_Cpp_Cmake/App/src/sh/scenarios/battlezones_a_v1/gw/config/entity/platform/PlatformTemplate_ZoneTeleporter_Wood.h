#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace battlezones_a_v1 {
class PlatformTemplate_ZoneTeleporter_Wood {
public:
	static EntityTemplate* getNew();

private:
    static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
