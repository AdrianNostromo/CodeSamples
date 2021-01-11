#pragma once

#include <base/gh.h>
#include <string>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
class TC_TeleporterPlatform_Up {
	pub static EntityTemplate* getOrCreateTemplate(unsigned int voxelSizeCM);

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase(unsigned int voxelSizeCM);

};
};
