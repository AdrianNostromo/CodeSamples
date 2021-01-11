#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace rpg3D {
class PlatformTemplates_PaintPad {
public:
    static EntityTemplate* getNew();

private:
    static Drawable3DCreateConfig_VoxelGrid* buildBase();
    static Drawable3DCreateConfig_VoxelGrid* buildPad();
    static Drawable3DCreateConfig_VoxelGrid* buildBrush();

};
};
