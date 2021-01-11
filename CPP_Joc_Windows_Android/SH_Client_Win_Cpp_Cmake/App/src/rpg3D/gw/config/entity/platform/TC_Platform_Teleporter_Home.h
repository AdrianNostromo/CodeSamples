#pragma once

#include <base/gh.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
// C == Customisable
class TC_Platform_Teleporter_Home {
	pub static EntityTemplate* getOrCreateTemplate(unsigned int voxelSizeCM);

    priv static Drawable3DCreateConfig_VoxelGrid* buildBase(unsigned int voxelSizeCM);
	priv static Drawable3DCreateConfig_VoxelGrid* buildIcon(unsigned int voxelSizeCM);

};
};
