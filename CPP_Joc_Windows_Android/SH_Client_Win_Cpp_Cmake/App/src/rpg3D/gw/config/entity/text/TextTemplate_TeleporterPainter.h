#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace rpg3D {
class TextTemplate_TeleporterPainter {
public:
	static EntityTemplate* getNew();

private:
    static Drawable3DCreateConfig_VoxelGrid* buildTextPainter();

};
};
