#pragma once

#include <base/gh.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
class TC_WallPillarA {
	pub static EntityTemplate* getOrCreateTemplate(unsigned int pillarSizeXYCM, unsigned int pillarSizeZCM);

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase(unsigned int pillarSizeXYCM, unsigned int pillarSizeZCM);
};
};
