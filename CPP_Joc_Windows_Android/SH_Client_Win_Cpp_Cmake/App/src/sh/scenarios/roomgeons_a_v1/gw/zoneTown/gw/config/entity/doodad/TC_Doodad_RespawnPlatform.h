#pragma once

#include <base/gh.h>
#include <string>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace roomgeons_a_v1 {
namespace Town {
// C == Customisable
class TC_Doodad_RespawnPlatform {
	pub static EntityTemplate* getOrCreateTemplate(unsigned int voxelSizeCM);

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase(unsigned int voxelSizeCM);

};
};
};
