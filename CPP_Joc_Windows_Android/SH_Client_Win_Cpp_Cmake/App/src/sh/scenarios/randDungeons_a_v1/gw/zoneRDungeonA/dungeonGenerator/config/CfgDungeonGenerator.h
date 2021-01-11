#pragma once

#include <base/gh.h>
#include <string>
#include <functional>
#include <memory>
#include <worldGame3D/gw/entity/template/ETemplateIdentifier.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>

class EntityTemplate;
class Visual3DModelMaterial;

namespace randDungeons_a_v1 {
namespace RDungeonA {
class CfgDungeonGenerator {pub dCtor(CfgDungeonGenerator);
	pub class RoomSizing {
		pub float smallAxis_size_min;
		pub float smallAxis_size_max;
		
		pub float bigAxis_scale_min;
		pub float bigAxis_scale_max;

		pub explicit RoomSizing(
			float smallAxis_size_min, float smallAxis_size_max,
			float bigAxis_scale_min, float bigAxis_scale_max)
			: smallAxis_size_min(smallAxis_size_min), smallAxis_size_max(smallAxis_size_max),
			bigAxis_scale_min(bigAxis_scale_min), bigAxis_scale_max(bigAxis_scale_max)
		{
			//void
		}
	};
	pub int zoneFloorsCount = 1;

	// This is the size of the visual tiles that are used to create the room.
	// Value is in centimeters.
	// {60, 100}; Templates for the specified size must exist.
	pub int tileSize_cm = 60;

	pub EntityTemplate* floor_tileableTemplate_gridXY = nullptr;
	pub Vector3Int floor_tileVoxelsCount{};
	pub ArrayList<Visual3DModelMaterial*>* floor_materialsList = nullptr;
	pub std::string floor_extraDataId_materialsList = "";
	pub std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> floor_tilesConfigList = nullptr;
	
	pub EntityTemplate* wall_tileableTemplate_linearX = nullptr;
	pub Vector3Int wall_tileVoxelsCount{};
	pub ArrayList<Visual3DModelMaterial*>* wall_materialsList = nullptr;
	pub std::string wall_extraDataId_materialsList = "";
	pub std::shared_ptr<graphics::TilingInformation::TileConfig> wall_tileConfig = nullptr;

	pub std::function<EntityTemplate *(int size_dm)>
		templateFactory_door = nullptr;

	pub base::ETemplateIdentifier templateIdentifier_teleporter_exitStairs{};
	pub base::ETemplateIdentifier templateIdentifier_teleporter_stairsDown{};
	pub base::ETemplateIdentifier templateIdentifier_teleporter_stairsUp{};

	pub base::ETemplateIdentifier templateIdentifier_mob{};

	// This must be equa or creater than doorsVisualTilesSize to allow for multi-tile doors.
	// This is used to scale the generated grid to the visual tiles grid.
	pub int gridCellTilesSize = 2;

	// The size of doors in visual tiles (the final ones) size.
	pub int doorsVisualTilesSize = 2;

	// zoneFloorGridSize_width * gridCellTilesSize = actual_tiles_grid_size.
	pub int zoneFloorGridSize_width = 20;
	pub int zoneFloorGridSize_height = 20;

	// Special rooms are bosses/ mob_groups or chest or something "special".
	// 2 rooms ill be used for in, out.
	pub int specialRoomsCount = 7;

	//[0.0f, 1.0f]
	// Lower values spreads the rooms more uniform and has a better fill ration.
	// High values causes some rooms to get close togetter.
	pub float specialRoomsSubGridPositionRandomisingStrength = 0.21f;

	// The sizes are preffered and small 1x1 rooms may occur and are used.
	pub RoomSizing inRoomSizing{
		3/*smallAxis_size_min*/, 4/*smallAxis_size_max*/,
		1.0f/*bigAxis_scale_min*/, 1.5f/*bigAxis_scale_max*/
	};
	pub RoomSizing outRoomSizing{
		3/*smallAxis_size_min*/, 4/*smallAxis_size_max*/,
		1.0f/*bigAxis_scale_min*/, 1.5f/*bigAxis_scale_max*/
	};
	pub RoomSizing specialRoomSizing{
		3/*smallAxis_size_min*/, 4/*smallAxis_size_max*/,
		1.0f/*bigAxis_scale_min*/, 1.5f/*bigAxis_scale_max*/
	};
	pub RoomSizing fillerRoomSizing{
		2/*smallAxis_size_min*/, 4/*smallAxis_size_max*/,
		1.0f/*bigAxis_scale_min*/, 1.5f/*bigAxis_scale_max*/
	};

    pub explicit CfgDungeonGenerator();

    pub virtual ~CfgDungeonGenerator();
};
};
};
