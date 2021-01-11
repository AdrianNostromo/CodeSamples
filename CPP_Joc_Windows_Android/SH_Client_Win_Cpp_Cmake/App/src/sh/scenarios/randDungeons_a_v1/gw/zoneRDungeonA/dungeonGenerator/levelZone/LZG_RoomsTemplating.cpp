#include "LZG_RoomsTemplating.h"
#include "roomElementsPopulator/RoomElementsPopulator.h"
#include "roomTemplater/RoomTemplater.h"
#include <memory>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

LZG_RoomsTemplating::LZG_RoomsTemplating(
	std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
	Random* rng)
	: super(
		config, zoneIndex,
		rng)
{
	//void
}

void LZG_RoomsTemplating::buildRoomConfigs() {
	if (config->gridCellTilesSize != config->doorsVisualTilesSize) {
		// The current algo needs gridCellTilesSize==doorsVisualTilesSize==2; Implement different methods when needed.
		throw LogicException(LOC);
	}

	levelZone->tileSizeM = config->tileSize_cm / 100.0f;
	levelZone->levelZoneRoomIndexesGrid = std::make_shared<Array2D<short>>(zoneGrid->getLengthA() * config->gridCellTilesSize, zoneGrid->getLengthB() * config->gridCellTilesSize);
	levelZone->levelZoneRoomIndexesGrid->memSetData(-1);

	for (int roomIndex = 0; roomIndex < roomifiedGridRoomZoneBoundsList.count(); roomIndex++) {
		Bounds2DInt& roomZoneSoftBounds = roomifiedGridRoomZoneBoundsList.getReference(roomIndex);

		bool isFirstZone = zoneIndex == 0 ? true : false;
		bool isLastZone = zoneIndex == config->zoneFloorsCount - 1 ? true : false;

		bool isLevelZoneEntranceRoom = roomIndex == 0 ? true : false;
		bool isLevelZoneExitRoom = roomIndex == 1 ? true : false;

		bool hasEntryPointStairs = false;
		if (isFirstZone && isLevelZoneEntranceRoom) {
			hasEntryPointStairs = true;
		}

		bool hasExitStairs = false;
		if (isLastZone && isLevelZoneExitRoom) {
			hasExitStairs = true;
		}
		
		bool hasStairsUp = false;
		if (!isFirstZone && isLevelZoneEntranceRoom) {
			hasStairsUp = true;
		}

		bool hasStairsDown = false;
		if (!isLastZone && isLevelZoneExitRoom) {
			hasStairsDown = true;
		}

		// Only the special locations rooms have mobs in them for now. Filler rooms have no mobs.
		int mobsCount = 0;
		if (roomIndex == 0) {
			// In room.
			mobsCount = 0;
		} else if (roomIndex == 1) {
			// Out room.
			mobsCount = 5;
		} else if (roomIndex < specialLocationsList->count()) {
			// Special rooms.
			mobsCount = 3;
		}

		std::unique_ptr<RoomElementsPopulator> roomElementsPopulator = std::make_unique<RoomElementsPopulator>();
		roomElementsPopulator->generate(
			roomIndex,
			config->gridCellTilesSize, config->doorsVisualTilesSize,
			roomZoneSoftBounds,
			zoneGrid,
			hasEntryPointStairs, hasExitStairs, hasStairsUp, hasStairsDown,
			mobsCount,
			levelZone->levelZoneRoomIndexesGrid
		);

		float tileSize_m = config->tileSize_cm / 100.0f;
		Vector3 roomOriginZonePos{
			(float)roomZoneSoftBounds.min.x * config->gridCellTilesSize * tileSize_m,
			(float)roomZoneSoftBounds.min.y * config->gridCellTilesSize * tileSize_m,
			0.0f * config->gridCellTilesSize * tileSize_m
		};

		std::unique_ptr<RoomTemplater> roomTemplater = std::make_unique<RoomTemplater>();
		roomTemplater->generate(
			config, roomOriginZonePos,
			roomElementsPopulator->result,
			zoneIndex
		);
		levelZone->roomsList.appendReference(roomTemplater->result->roomContent);

	}
}

LZG_RoomsTemplating::~LZG_RoomsTemplating() {
	//void
}
