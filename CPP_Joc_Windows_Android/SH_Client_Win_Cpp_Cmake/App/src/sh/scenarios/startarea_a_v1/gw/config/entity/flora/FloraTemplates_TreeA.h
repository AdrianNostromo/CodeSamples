#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <string>
#include <base/list/Array3D.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace startarea_a_v1 {
class FloraTemplates_TreeA {
public:
    static EntityTemplate* GetNew();

private:
    static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
