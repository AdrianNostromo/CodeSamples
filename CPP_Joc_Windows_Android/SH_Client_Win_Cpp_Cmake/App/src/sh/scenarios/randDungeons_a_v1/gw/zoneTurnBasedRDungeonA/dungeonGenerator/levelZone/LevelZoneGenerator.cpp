#include "LevelZoneGenerator.h"
#include "../config/CfgDungeonGenerator.h"
#include <base/math/Math.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <cstdio>
#include <base/math/Random.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

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
	buildSpecialLocationsListAndZoneGrid();

	//logSpecialLocationsAndRoadsGrid();

	// Build zoneGrid paths.
	buildRoads();

	//logSpecialLocationsAndRoadsGrid();
	
	// Chose the most distant slots for in and out.
	SpecialLocationsDistPair specialLocationsDistPair{};
	findInOutLocations(specialLocationsDistPair);
	if (specialLocationsDistPair.longestLocationsRoad < 0) {
		throw LogicException(LOC);
	}

	// Move special locations so that the main room is at index 0 and exits are at the following indexes.
	if (specialLocationsDistPair.locationIndexA != 0) {
		// Also swap the indexes in the zoneGrid.
		ZoneGridCell& gCellA = zoneGrid->getReference(
			specialLocationsList->getReference(0).pos.x,
			specialLocationsList->getReference(0).pos.y
		);
		ZoneGridCell& gCellB = zoneGrid->getReference(
			specialLocationsList->getReference(specialLocationsDistPair.locationIndexA).pos.x,
			specialLocationsList->getReference(specialLocationsDistPair.locationIndexA).pos.y
		);
		int t = gCellA.roomIndex;
		gCellA.roomIndex = gCellB.roomIndex;
		gCellB.roomIndex = t;

		// Swap [0] with [locationIndexA];
		specialLocationsList->swap(0, specialLocationsDistPair.locationIndexA);

		if (specialLocationsDistPair.locationIndexB == 0) {
			// After the swap this is correct.
			specialLocationsDistPair.locationIndexB = specialLocationsDistPair.locationIndexA;
		}
	}
	if (specialLocationsDistPair.locationIndexB != 1) {
		ZoneGridCell& gCellA = zoneGrid->getReference(
			specialLocationsList->getReference(1).pos.x,
			specialLocationsList->getReference(1).pos.y
		);
		ZoneGridCell& gCellB = zoneGrid->getReference(
			specialLocationsList->getReference(specialLocationsDistPair.locationIndexB).pos.x,
			specialLocationsList->getReference(specialLocationsDistPair.locationIndexB).pos.y
		);
		int t = gCellA.roomIndex;
		gCellA.roomIndex = gCellB.roomIndex;
		gCellB.roomIndex = t;

		specialLocationsList->swap(1, specialLocationsDistPair.locationIndexB);
	}

	//logSpecialLocationsAndRoadsGrid();

	roomifyGrid();

	//logSpecialLocationsAndRoadsGrid();

	////logRooms();

	//// Build the rooms.
	buildRoomConfigs();
	
	return levelZone;
}

LevelZoneGenerator::~LevelZoneGenerator() {
	//void
}
