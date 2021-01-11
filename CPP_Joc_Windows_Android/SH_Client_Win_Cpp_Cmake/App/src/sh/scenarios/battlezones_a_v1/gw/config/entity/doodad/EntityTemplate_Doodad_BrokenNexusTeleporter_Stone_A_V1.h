#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace battlezones_a_v1 {
class EntityTemplate_Doodad_BrokenNexusTeleporter_Stone_A_V1 {
public:
    static EntityTemplate* GetNew();

private:
    static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
