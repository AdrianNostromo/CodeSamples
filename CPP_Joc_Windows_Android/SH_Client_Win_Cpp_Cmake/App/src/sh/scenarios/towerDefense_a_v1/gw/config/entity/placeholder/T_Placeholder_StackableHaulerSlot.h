#pragma once

#include <base/gh.h>
#include <string>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace towerDefense_a_v1 {
class T_Placeholder_StackableHaulerSlot {
	pub static std::string ID;
	pub static EntityTemplate* TEMPLATE;

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};