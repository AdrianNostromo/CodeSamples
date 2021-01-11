#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace rpg3D {
class Template_Doodad_BrokenTeleporter_Stone_A_V1 {
public:
	static EntityTemplate* getNew();

private:
    static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
