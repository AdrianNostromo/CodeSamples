#pragma once

#include <base/gh.h>
#include <string>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>

class EntityTemplate;
class Visual3DModelMaterial;
class Drawable3DCreateConfig_VoxelGrid;

namespace rpg3D {
class T_Wall_A_TiledX6dm {
	pub static std::string ID;

	pub static std::string DATA_KEY_materials;

	pub static Vector3Int TileVoxelsCount;

	pub static Array3D<signed char>* grid;
	pub static ArrayList<Visual3DModelMaterial*>* materialsList;

	pub static EntityTemplate* TEMPLATE;

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
