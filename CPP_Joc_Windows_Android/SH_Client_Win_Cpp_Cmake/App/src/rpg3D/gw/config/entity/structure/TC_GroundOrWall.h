#pragma once

#include <base/gh.h>
#include <string>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <base/list/Array1D.h>
#include <base/sp.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;
class Visual3DModelMaterial;
namespace base {
	class IWrappedValue;
};

namespace rpg3D {
// C == Customisable
class TC_GroundOrWall {
	pub enum MaterialIndices { Empty = -1, GrassA = 0, GrassB = 1, StoneA = 2, StoneB = 3, StoneC = 4, StoneD = 5, DirtA = 6 };
	pub enum UseType {Floor=0, Wall=1};
	pub enum PhysicsType {None=0, Floor_TiledGrid2D=1, Wall_TiledLinear=2, Wall_TiledGrid2D=3};

	priv static bool IsStaticsInit;

	pub static std::string BaseId;
	pub static std::string DATA_KEY_materials;

	// This is used for physics size and is the same as the TileVoxelsCount_...;
	pub static Vector2Int GridSizeXY;
	pub static Vector3Int TileVoxelsCount_1Layer;
	pub static Vector3Int TileVoxelsCount_2Layers;

	pub static Array3D<signed char>* getOrCreateGridGrassA();
	pub static Array3D<signed char>* getOrCreateGridGrassB();
	pub static Array3D<signed char>* getOrCreateGridStoneA();
	pub static Array3D<signed char>* getOrCreateGridStoneB();
	pub static Array3D<signed char>* getOrCreateGridStoneC();
	pub static Array3D<signed char>* getOrCreateGridStoneD();

	pub static Array3D<signed char>* getOrCreateGrid_None_None();
	pub static Array3D<signed char>* getOrCreateGridGrassADirtA();
	pub static Array3D<signed char>* getOrCreateGridGrassBDirtA();

	pub static ArrayList<Visual3DModelMaterial*>* getOrCreateMaterials();

	pub static EntityTemplate* getOrCreateTemplate(
		UseType useType, PhysicsType physicsType, Vector3Int& tileVoxelsGridSize,
		unsigned int voxelAndTileSizeCM, float heightScale);
	pub static EntityTemplate* GetOrCreateTemplateFromConfigParams(sp<Array1D<std::shared_ptr<base::IWrappedValue>>>& configValues);

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase(unsigned int voxelAndTileSizeCM, Vector3Int& tileVoxelsGridSize, float heightScale, int/*UseType*/ useType);

};
};
