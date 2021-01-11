#include "LZG_GridRomify.h"
#include "util/PathingAlgo.h"
#include <iostream>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

const int LZG_GridRomify::RoomType::IN = 1;
const int LZG_GridRomify::RoomType::OUT = 2;
const int LZG_GridRomify::RoomType::SPECIAL = 3;
const int LZG_GridRomify::RoomType::FILLER = 4;

LZG_GridRomify::LZG_GridRomify(
	std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
	Random* rng)
	: super(
		config, zoneIndex,
		rng)
{
	//void
}

void LZG_GridRomify::roomifyGrid() {
	clearExistingRoadDistance(0);

	//logZoneGrid();

	// Rooms builder must not intersect close unconnected paths.
	// Build special rooms first. Starting with in,out first.
	// Special rooms try to grow opposite of the main road.
	for (int roomIndex = 0; roomIndex < specialLocationsList->count(); roomIndex++) {
		SpecialLocation& pgSource = specialLocationsList->getReference(roomIndex);

		// Compute a preffered room size.
		int roomType;
		if (roomIndex == 0) {
			roomType = RoomType::IN;
		} else if (roomIndex == 1) {
			roomType = RoomType::OUT;
		} else {
			roomType = RoomType::SPECIAL;
		}

		CfgDungeonGenerator::RoomSizing* roomSizing = nullptr;
		if (roomType == RoomType::IN) {
			roomSizing = &config->inRoomSizing;
		} else if (roomType == RoomType::OUT) {
			roomSizing = &config->outRoomSizing;
		} else if (roomType == RoomType::SPECIAL) {
			roomSizing = &config->specialRoomSizing;
		} else {
			throw LogicException(LOC);
		}

		// Generate a preffered room size. The room will be equal or smaller, it will never be larger.
		int roomSizeSmallAxis = rng->nextInt(roomSizing->smallAxis_size_min, roomSizing->smallAxis_size_max);
		int roomSizeBigAxis = Math::floor(roomSizeSmallAxis * rng->nextFloat(roomSizing->bigAxis_scale_min, roomSizing->bigAxis_scale_max));

		//std::cout << "Generate room. roomIndex: " << Math::encodeNumberToSingleChar(roomIndex) << " x: " << pgSource.pos.x << " y: " << pgSource.pos.y << " roomSizeSmallAxis: " << roomSizeSmallAxis << " roomSizeBigAxis: " << roomSizeBigAxis << std::endl;

		RoomGrower* roomGrower = new RoomGrower();
		Bounds2DInt roomBounds{};
		roomGrower->autoGrowRoom(
			roomIndex, pgSource.pos, roomSizeSmallAxis, roomSizeBigAxis,
			zoneGrid, RoomGrower::LeastRoad, roomBounds, rng
		);
		delete roomGrower;

		if ((roomBounds.getLenX() == roomSizeSmallAxis && roomBounds.getLenY() == roomSizeBigAxis) || (roomBounds.getLenX() == roomSizeBigAxis && roomBounds.getLenY() == roomSizeSmallAxis)) {
			//std::cout << "Specal room generated (exact size). roomIndex: " << Math::encodeNumberToSingleChar(roomIndex) << " x: " << roomBounds.min.x << " y: " << roomBounds.min.y << " w: " << roomBounds.getLenX() << " h: " << roomBounds.getLenY() << std::endl;
		} else {
			std::cout << "Specal room generated (missed size). roomIndex: " << Math::encodeNumberToSingleChar(roomIndex) << " x: " << roomBounds.min.x << " y: " << roomBounds.min.y << " w: " << roomBounds.getLenX() << " h: " << roomBounds.getLenY() << std::endl;
		}

		for (int gX = roomBounds.min.x; gX < roomBounds.max.x; gX++) {
			for (int gY = roomBounds.min.y; gY < roomBounds.max.y; gY++) {
				zoneGrid->getReference(gX, gY).roomIndex = roomIndex;
			}
		}

		roomifiedGridRoomZoneBoundsList.append_emplace(roomBounds);

		//logZoneGrid();
	}

	//logZoneGrid();
	
	// Build intermedieary rooms.
	// Init pathing_b;
	// Use pathingDistIndex of 1 because 0 is used by the individual rooms builder logic.
	PathingAlgo* pathingAlgo = new PathingAlgo(
		true/*processExistingRoadsOnly*/, 1/*pathingDistIndex*/, nullptr/*cbOnNewPathGCell*/,
		zoneGrid, getRoadGPositionsList()
	);
	
	bool isInit = false;
	int roomIndex = specialLocationsList->count();
	while (true) {
		if (!isInit) {
			isInit = true;

			pathingAlgo->init(specialLocationsList->getReference(0).pos/*startPos*/);
		} else {
			pathingAlgo->tickPathing();
		}

		if (!pathingAlgo->isInProgress()) {
			// No more existing road gCells to path.
			break;
		}

		// Enumerate the new pathed road gCells and handle room creation if road and has roomIndex==-1.
		CfgDungeonGenerator::RoomSizing* roomSizing = &config->fillerRoomSizing;

		ArrayList<Vector2Int>* nextPathAlgorithmLocations = pathingAlgo->peekNextPathAlgorithmLocations();
		for(int i=0;i< nextPathAlgorithmLocations->count();i++) {
			Vector2Int& gPos = nextPathAlgorithmLocations->getReference(i);
			ZoneGridCell& cGCell = zoneGrid->getReference(gPos);

			if (cGCell.roadDirBitMap == 0) {
				// The pathing algo must (is configured to) path only roads.
				throw LogicException(LOC);
			}
			if (cGCell.roomIndex >= 0) {
				// Already a room, skip.
				// Not roomIndex==-1.
				continue;
			}

			// cGCell is a road and roomIndex==-1;

			// Generate a preffered room size. The room will be equal or smaller, it will never be larger.
			int roomSizeSmallAxis = rng->nextInt(roomSizing->smallAxis_size_min, roomSizing->smallAxis_size_max);
			int roomSizeBigAxis = Math::floor(roomSizeSmallAxis * rng->nextFloat(roomSizing->bigAxis_scale_min, roomSizing->bigAxis_scale_max));

			//std::cout << "Generate room. roomIndex: " << Math::encodeNumberToSingleChar(roomIndex) << " x: " << gPos.x << " y: " << gPos.y << " roomSizeSmallAxis: " << roomSizeSmallAxis << " roomSizeBigAxis: " << roomSizeBigAxis << std::endl;

			RoomGrower* roomGrower = new RoomGrower();
			Bounds2DInt roomBounds{};
			roomGrower->autoGrowRoom(
				roomIndex, gPos/*startPos*/, roomSizeSmallAxis, roomSizeBigAxis,
				zoneGrid, RoomGrower::MostRoad, roomBounds, rng
			);
			delete roomGrower;

			if ((roomBounds.getLenX() == roomSizeSmallAxis && roomBounds.getLenY() == roomSizeBigAxis) || (roomBounds.getLenX() == roomSizeBigAxis && roomBounds.getLenY() == roomSizeSmallAxis)) {
				//std::cout << "Filler room generated (exact size). roomIndex: " << Math::encodeNumberToSingleChar(roomIndex) << " x: " << roomBounds.min.x << " y: " << roomBounds.min.y << " w: " << roomBounds.getLenX() << " h: " << roomBounds.getLenY() << std::endl;
			} else {
				//std::cout << "Filler room generated (missed size). roomIndex: " << Math::encodeNumberToSingleChar(roomIndex) << " x: " << roomBounds.min.x << " y: " << roomBounds.min.y << " w: " << roomBounds.getLenX() << " h: " << roomBounds.getLenY() << std::endl;
			}

			for (int gX = roomBounds.min.x; gX < roomBounds.max.x; gX++) {
				for (int gY = roomBounds.min.y; gY < roomBounds.max.y; gY++) {
					zoneGrid->getReference(gX, gY).roomIndex = roomIndex;
				}
			}

			roomifiedGridRoomZoneBoundsList.append_emplace(roomBounds);

			roomIndex++;

			//logZoneGrid();
		}
	}
	delete pathingAlgo;

	//logZoneGrid(true, true);
}

LZG_GridRomify::~LZG_GridRomify() {
	//void
}
