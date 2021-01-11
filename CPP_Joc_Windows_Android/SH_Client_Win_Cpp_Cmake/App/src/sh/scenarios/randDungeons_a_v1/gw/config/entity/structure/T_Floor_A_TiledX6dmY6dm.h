#pragma once

#include <base/gh.h>
#include <string>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;
class Visual3DModelMaterial;

namespace randDungeons_a_v1 {
class T_Floor_A_TiledX6dmY6dm {
	pub static std::string ID;

	pub static std::string DATA_KEY_materials;

	pub static Vector3Int TileVoxelsCount;

	pub static Array3D<signed char>* gridA;
	pub static Array3D<signed char>* gridB;
	pub static ArrayList<Visual3DModelMaterial*>* materialsList;

	pub static EntityTemplate* TEMPLATE;

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
