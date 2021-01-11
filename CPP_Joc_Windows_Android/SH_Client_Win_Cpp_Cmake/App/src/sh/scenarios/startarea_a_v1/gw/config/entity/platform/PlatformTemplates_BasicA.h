#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace startarea_a_v1 {
class PlatformTemplates_BasicA {
public:
    static EntityTemplate* GetNew();

private:
    static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
