#pragma once

#include <base/gh.h>
#include <string>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
class T_Item_Coin_Gold_A_V1 {
	pub static std::string ID;
	pub static EntityTemplate* TEMPLATE;

	pub static const float GROUND_PLACEMENT_OFFSET_Z;

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
