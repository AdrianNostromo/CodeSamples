#pragma once

#include <base/gh.h>
#include <string>
#include <base/map/Map1D.h>
#include <base/list/Array3D.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
class Template_Text_Custom {
	pub static std::string ID_Prefix;

	// Space is 1 voxel wide only.
	// Text is left aligned.
	// First '\r' causes a new line. Multiple '\r' after cause a 1 voxel lowering only.
	pub static EntityTemplate* GetNewOrExistingTemplate(std::string text, float voxelSize);

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase(std::string& text, float voxelSize);
};
};
