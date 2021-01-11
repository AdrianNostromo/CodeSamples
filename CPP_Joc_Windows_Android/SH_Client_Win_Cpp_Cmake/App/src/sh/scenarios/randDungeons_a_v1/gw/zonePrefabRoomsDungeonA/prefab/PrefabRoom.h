#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/math/Vector3.h>
#include <base/math/Vector2.h>
#include <base/math/Vector3Int.h>
#include <string>
#include <base/list/Array2D.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>

class EntityTemplate;
class Visual3DModelMaterial;
namespace rpg3D {
	class EffectPayload;
};

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
class PrefabRoom {pub dCtor(PrefabRoom);
	pub int tileSize_cm;

	pub std::shared_ptr<Array2D<signed char>> floorTilesGrid = nullptr;

	// Real pos, not grid pos.
	pub ArrayList<Vector2> teleporterGPosList{};

	pub EntityTemplate* floor_tileableTemplate_gridXY = nullptr;
	pub Vector3Int floor_tileVoxelsCount{};
	pub ArrayList<Visual3DModelMaterial*>* floor_materialsList = nullptr;
	pub std::string floor_extraDataId_materialsList = "";
	pub std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> floor_tilesConfigList = nullptr;

	pub explicit PrefabRoom();
			
	pub virtual ~PrefabRoom();
};
};
};
