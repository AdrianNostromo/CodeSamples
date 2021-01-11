#pragma once

#include <base/gh.h>
#include <string>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace randDungeons_a_v1 {
class T_Doorway_A_VariableX1dm {
	pub static std::string ID_WITH_PLACEHOLDERS;

	pub static EntityTemplate* getOrCreate(int size_dm);
	priv static Drawable3DCreateConfig_VoxelGrid* buildBase(int size_dm);

};
};
