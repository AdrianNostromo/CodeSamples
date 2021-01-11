#pragma once

#include <base/gh.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
class T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1 {
	pub static EntityTemplate* getOrCreateTemplate();

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};