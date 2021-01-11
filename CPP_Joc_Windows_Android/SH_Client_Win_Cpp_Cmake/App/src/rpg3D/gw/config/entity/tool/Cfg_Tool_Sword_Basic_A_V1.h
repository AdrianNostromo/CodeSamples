#pragma once

#include <base/gh.h>
#include <string>

class Drawable3DCreateConfig_VoxelGrid;
namespace rpg3D {
	class ToolConfigBase;
};

namespace rpg3D {
class Cfg_Tool_Sword_Basic_A_V1 {
	pub static ToolConfigBase* getOrCreateTemplate();

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();
};
};