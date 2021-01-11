#pragma once

#include <base/gh.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
class T_Projectile_Bolt_Basic_A_V1 {
	pub static EntityTemplate* getOrCreateTemplate();

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
