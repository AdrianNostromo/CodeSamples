#include "RoomElementsPopulator.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

RoomElementsPopulator::RoomElementsPopulator()
	: super()
{
	//void
}

void RoomElementsPopulator::generate(
	int roomIndex,
	int gridCellTilesSize, int doorsVisualTilesSize,
	Bounds2DInt& roomZoneSoftBounds,
	Array2D<ZoneGridCell>* zoneGrid,
	bool hasEntryPointStairs, bool hasExitStairs, bool hasStairsUp, bool hasStairsDown,
	int mobsCount,
	std::shared_ptr<Array2D<short>>& oLevelZoneRoomIndexesGrid)
{
	super::generate(
		roomIndex,
		gridCellTilesSize, doorsVisualTilesSize,
		roomZoneSoftBounds,
		zoneGrid,
		hasEntryPointStairs, hasExitStairs, hasStairsUp, hasStairsDown,
		mobsCount,
		oLevelZoneRoomIndexesGrid
	);
	

	// Log the grid once for each side.
	//logTilesGrid(roomTilesInfoGrid);

	prepareSideEementsInfoGrid();

	// Log the grid once for each side.
	//logTilesGrid(roomTilesInfoGrid);

	// This is n tiles grid coords and doesn't have the room pos.
	combineSideElementsAndCreateList();

	// Create a 2d int grid (-1 empty, 0 used) for the ground. This will be passed to the entity voxels generator. Use a s_p.
	createFloorTilingGrid();

	placeTeleporters();

	placeMobsGroup();

	// Log the grid once for each side.
	// logTilesGrid();
}

RoomElementsPopulator::~RoomElementsPopulator() {
	reservedDisposeIfNeeded();// Note1001
}
