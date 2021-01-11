#pragma once

#include <base/gh.h>
#include <string>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;

namespace towerDefense_a_v1 {
class Template_Turret_CubesCannon_A_V1 {
	pub static std::string ID;
	pub static EntityTemplate* TEMPLATE;

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();
	priv static Drawable3DCreateConfig_VoxelGrid* buildTurret();
	priv static Drawable3DCreateConfig_VoxelGrid* buildBarrel();
	priv static Drawable3DCreateConfig_VoxelGrid* buildProjectile();

};
};
