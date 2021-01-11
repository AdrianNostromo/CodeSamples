#pragma once

#include <base/gh.h>
#include <string>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace customRoomgeons {
class T_WallElement_A_V1 {
	pub static std::string ID;
	pub static EntityTemplate* TEMPLATE;

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
