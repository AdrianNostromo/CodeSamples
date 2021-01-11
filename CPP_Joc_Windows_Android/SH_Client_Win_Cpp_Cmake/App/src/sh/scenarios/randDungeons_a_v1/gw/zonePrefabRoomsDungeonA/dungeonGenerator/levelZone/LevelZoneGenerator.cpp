#include "LevelZoneGenerator.h"
#include "../config/CfgDungeonGenerator.h"
#include <base/math/Math.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <cstdio>
#include <base/math/Random.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::PrefabRoomsDungeonA;

LevelZoneGenerator::LevelZoneGenerator(
	std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
	Random* rng)
	: super(
		config, zoneIndex,
		rng)
{
	//void
}

std::shared_ptr<DungeonContent::LevelZone> LevelZoneGenerator::generate() {
	//asd_g;// take the first prefab room and place it as a start room.
	//asd_g;// it must also have a teleporter placement location.
	//asd_g;// place 1 start room and 1 end room.

	// Add a empty room.
	float tileSize_m = 0.6f;

	//asd_g;// need a wall creation.
	//asd_g;/// maybe re-use the other generator classes for walls and doors placements.

	if (zoneIndex == 0) {
		{
			Vector3 roomOriginZonePos{
					0.0f * tileSize_m,
					0.0f * tileSize_m,
					0.0f * tileSize_m
			};
			std::shared_ptr<DungeonContent::LevelZone::Room> roomContent = std::make_shared<DungeonContent::LevelZone::Room>(roomOriginZonePos);
			PrefabRoom* prefabRoom = config->prefabRoomsList->getPointer(0);
			placePrefabRoom(
				prefabRoom,
				true/*hasEntryPointStairs*/, false/*hasExitStairs*/, false/*hasStairsUp*/, false/*hasStairsDown*/,
				roomContent
			);
			levelZone->roomsList.appendReference(roomContent);
		}
		{
			Vector3 roomOriginZonePos{
					7.0f * tileSize_m,
					0.0f * tileSize_m,
					0.0f * tileSize_m
			};
			std::shared_ptr<DungeonContent::LevelZone::Room> roomContent = std::make_shared<DungeonContent::LevelZone::Room>(roomOriginZonePos);
			PrefabRoom* prefabRoom = config->prefabRoomsList->getPointer(1);
			placePrefabRoom(
				prefabRoom,
				false/*hasEntryPointStairs*/, false/*hasExitStairs*/, false/*hasStairsUp*/, false/*hasStairsDown*/,
				roomContent
			);
			levelZone->roomsList.appendReference(roomContent);
		}
		{
			Vector3 roomOriginZonePos{
					14.0f * tileSize_m,
					0.0f * tileSize_m,
					0.0f * tileSize_m
			};
			std::shared_ptr<DungeonContent::LevelZone::Room> roomContent = std::make_shared<DungeonContent::LevelZone::Room>(roomOriginZonePos);
			PrefabRoom* prefabRoom = config->prefabRoomsList->getPointer(0);
			placePrefabRoom(
				prefabRoom,
				false/*hasEntryPointStairs*/, false/*hasExitStairs*/, false/*hasStairsUp*/, true/*hasStairsDown*/,
				roomContent
			);
			levelZone->roomsList.appendReference(roomContent);
		}
	}else if(zoneIndex == 1) {
		{
			Vector3 roomOriginZonePos{
					0.0f * tileSize_m,
					0.0f * tileSize_m,
					0.0f * tileSize_m
			};
			std::shared_ptr<DungeonContent::LevelZone::Room> roomContent = std::make_shared<DungeonContent::LevelZone::Room>(roomOriginZonePos);
			PrefabRoom* prefabRoom = config->prefabRoomsList->getPointer(0);
			placePrefabRoom(
				prefabRoom,
				false/*hasEntryPointStairs*/, false/*hasExitStairs*/, true/*hasStairsUp*/, false/*hasStairsDown*/,
				roomContent
			);
			levelZone->roomsList.appendReference(roomContent);
		}
		{
			Vector3 roomOriginZonePos{
					7.0f * tileSize_m,
					0.0f * tileSize_m,
					0.0f * tileSize_m
			};
			std::shared_ptr<DungeonContent::LevelZone::Room> roomContent = std::make_shared<DungeonContent::LevelZone::Room>(roomOriginZonePos);
			PrefabRoom* prefabRoom = config->prefabRoomsList->getPointer(1);
			placePrefabRoom(
				prefabRoom,
				false/*hasEntryPointStairs*/, false/*hasExitStairs*/, false/*hasStairsUp*/, false/*hasStairsDown*/,
				roomContent
			);
			levelZone->roomsList.appendReference(roomContent);
		}
		{
			Vector3 roomOriginZonePos{
					14.0f * tileSize_m,
					0.0f * tileSize_m,
					0.0f * tileSize_m
			};
			std::shared_ptr<DungeonContent::LevelZone::Room> roomContent = std::make_shared<DungeonContent::LevelZone::Room>(roomOriginZonePos);
			PrefabRoom* prefabRoom = config->prefabRoomsList->getPointer(0);
			placePrefabRoom(
				prefabRoom,
				false/*hasEntryPointStairs*/, true/*hasExitStairs*/, false/*hasStairsUp*/, false/*hasStairsDown*/,
				roomContent
			);
			levelZone->roomsList.appendReference(roomContent);
		}
	}else {
		throw LogicException(LOC);
	}

	// Build room walls and doors and stuff.
	//asd_g;

	// Place teleporter.
	//asd_g;


	return levelZone;
}

void LevelZoneGenerator::placePrefabRoom(
	PrefabRoom* prefabRoom,
	bool hasEntryPointStairs, bool hasExitStairs, bool hasStairsUp, bool hasStairsDown,
	std::shared_ptr<DungeonContent::LevelZone::Room> roomContent)
{
	float gCellSizeM = prefabRoom->tileSize_cm / 100.0f;

	{
		Vector2Int prefabRoomFloorTilesGridSize{ prefabRoom->floorTilesGrid->getLengthA(), prefabRoom->floorTilesGrid->getLengthB() };
		
		std::shared_ptr<Array2D<signed char>> tilingGrid = std::make_shared<Array2D<signed char>>(prefabRoomFloorTilesGridSize.x, prefabRoomFloorTilesGridSize.y);
		for (int xx = 0; xx < prefabRoomFloorTilesGridSize.x; xx++) {
			for (int yy = 0; yy < prefabRoomFloorTilesGridSize.y; yy++) {
				signed char tileType = prefabRoom->floorTilesGrid->getDirect(xx, yy);
				
				tilingGrid->setDirect(xx, yy, tileType);
			}
		}

		std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
			prefabRoom->floor_tileVoxelsCount,
			-1/*enabledSidesTriangulationBitMask*/,
			prefabRoom->floor_tilesConfigList,
			tilingGrid,
			nullptr/*gridGeometryOffset*/
		);

		Vector3 elementPos{ 0.0f, 0.0f, 0.0f };

		roomContent->structureElementsList.append_emplace(
			elementPos, 0.0f,
			prefabRoom->floor_tileableTemplate_gridXY->nSpace, prefabRoom->floor_tileableTemplate_gridXY->id,
			prefabRoom->floor_materialsList, prefabRoom->floor_extraDataId_materialsList,
			tilingInformation/*tilingInformation*/
		);
	}

	{
		std::string t_nSpace = "";
		std::string t_id = "";
		std::shared_ptr<std::string> instanceId = nullptr;
		int teleportTargetZoneLevelIndex = -1;
		std::string teleportTargetEntityInstanceID = "";
		if (hasEntryPointStairs) {
			t_nSpace = config->templateIdentifier_teleporter_exitStairs.nSpace;
			t_id = config->templateIdentifier_teleporter_exitStairs.id;

			instanceId = EntityIds::teleportDungeonEntryPoint;
			teleportTargetZoneLevelIndex = -1;
			teleportTargetEntityInstanceID = "";
		}else if (hasExitStairs) {
			t_nSpace = config->templateIdentifier_teleporter_exitStairs.nSpace;
			t_id = config->templateIdentifier_teleporter_exitStairs.id;

			instanceId = EntityIds::teleporterExitDungeon;
			teleportTargetZoneLevelIndex = -1;
			teleportTargetEntityInstanceID = "";
		}else if (hasStairsDown) {
			t_nSpace = config->templateIdentifier_teleporter_stairsDown.nSpace;
			t_id = config->templateIdentifier_teleporter_stairsDown.id;

			instanceId = std::make_shared<std::string>(EntityIds::teleportPrefix + "down1");
			teleportTargetZoneLevelIndex = zoneIndex + 1;
			teleportTargetEntityInstanceID = EntityIds::teleportPrefix + "up1";
		}else if (hasStairsUp) {
			t_nSpace = config->templateIdentifier_teleporter_stairsUp.nSpace;
			t_id = config->templateIdentifier_teleporter_stairsUp.id;

			instanceId = std::make_shared<std::string>(EntityIds::teleportPrefix + "up1");
			teleportTargetZoneLevelIndex = zoneIndex - 1;
			teleportTargetEntityInstanceID = EntityIds::teleportPrefix + "down1";
		}

		if (!t_nSpace.empty()) {
			if (prefabRoom->teleporterGPosList.count() != 1) {
				// The prefab room has no or too many teleporter locations.
				throw LogicException(LOC);
			}

			Vector2& teleporterGPos = prefabRoom->teleporterGPosList.getReference(0);

			Vector3 elementPos{ teleporterGPos.x * gCellSizeM, teleporterGPos.y * gCellSizeM, 0.0f };

			roomContent->teleportersList.append_emplace(
				elementPos, 0.0f,
				t_nSpace, t_id,
				instanceId,
				teleportTargetZoneLevelIndex, teleportTargetEntityInstanceID
			);
		}else {
			if (prefabRoom->teleporterGPosList.count() > 0) {
				// The prefab rooms requires one or more teleporters.
				throw LogicException(LOC);
			}
		}
	}
}

LevelZoneGenerator::~LevelZoneGenerator() {
	//void
}
