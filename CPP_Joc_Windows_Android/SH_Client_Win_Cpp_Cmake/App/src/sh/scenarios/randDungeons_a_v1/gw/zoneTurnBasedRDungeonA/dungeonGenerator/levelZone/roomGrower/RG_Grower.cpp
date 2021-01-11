#include "RG_Grower.h"
#include <base/list/Array1D.h>
#include <base/math/Math.h>
#include <base/math/Random.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

RG_Grower::RG_Grower()
	: super()
{
	//void
}

void RG_Grower::autoGrowRoom(
	int roomIndex, Vector2Int& startZonePos, int preferredRoomSize_smallAxis, int preferredRoomSize_bigAxis,
	Array2D<ZoneGridCell>* zoneGrid, RoomGrowType roomGrowType, Bounds2DInt& oFinalRoomBounds, Random* rng)
{
	super::autoGrowRoom(
		roomIndex, startZonePos, preferredRoomSize_smallAxis, preferredRoomSize_bigAxis,
		zoneGrid, roomGrowType, oFinalRoomBounds, rng
	);

	// Fill the intermediary data grid.
	initRoomWallDistancesGrid();

	//logDistancesGrid();

	initPathingAlgo(roomIndex, startZonePos);

	// This is used to make a list of the best rooms. Generated rooms are always equal or smaller.
	// This is the area diferrence.
	// Val is >= 0; closer to zero is better. 0 is the exact requested size.
	int finalZoneRoomCandidatesDistanceFromOptimal = -1;
	int finalZoneRoomCandidatesRoadsCost = -1;
	std::shared_ptr<ArrayList<Bounds2DInt>> finalZoneRoomCandidatesList = std::make_shared<ArrayList<Bounds2DInt>>();
	
	// Do the logic loop until roads net ends or all current A* road g_cells exit te algo grid.
	while (true) {
		// Run the rooms creator algorithm because initPathingAlgo(...) did a manual roads tick for the room origin.
		// Find all rooms available for the current roomsRoadCost;
		// This is used in case the requested size doesn't fit.
		int tZoneRoomCandidatesDistanceFromOptimal = -1;
		std::shared_ptr<ArrayList<Bounds2DInt>> tZoneRoomCandidatesList = std::make_shared<ArrayList<Bounds2DInt>>();

		int roadGCellsProcessedCount = tickRoomsFinderAlgo(
			tZoneRoomCandidatesDistanceFromOptimal, *tZoneRoomCandidatesList
		);

		if (tZoneRoomCandidatesList->count() > 0) {
			// Some rooms found. Use size compare.

			if (finalZoneRoomCandidatesDistanceFromOptimal == -1/*first_valid_list*/
				|| tZoneRoomCandidatesDistanceFromOptimal < finalZoneRoomCandidatesDistanceFromOptimal /*better_size*/ 
				|| (roomGrowType == MostRoad /*prefers_more_road*/ && tZoneRoomCandidatesDistanceFromOptimal == finalZoneRoomCandidatesDistanceFromOptimal/*same_size*/))
			{
				// Better size always save them.
				// Also save if longer road prefferred and same save, save them also.

				finalZoneRoomCandidatesDistanceFromOptimal = tZoneRoomCandidatesDistanceFromOptimal;
				finalZoneRoomCandidatesRoadsCost = getCurrentPathingRoadDist();
				finalZoneRoomCandidatesList = tZoneRoomCandidatesList;
			}
		}

		if (roadGCellsProcessedCount == 0) {
			// All road gCells processed or all available are outside of the algo bounds.
			break;
		}

		if (roomGrowType == LeastRoad /*preffer_least_road*/ && finalZoneRoomCandidatesDistanceFromOptimal == 0/*required_size_found*/) {
			break;
		}

		bool isPathingAlgoActive = tickPathingAlgo();
		if (!isPathingAlgoActive) {
			break;
		}
	}

	if (finalZoneRoomCandidatesList->count() == 0) {
		// The algo always produces a room, even if 1x1 in size.
		throw LogicException(LOC);
	}
	// Chose a random optimal room.
	oFinalRoomBounds.set(finalZoneRoomCandidatesList->getReference(rng->nextInt(finalZoneRoomCandidatesList->count() - 1)));

	// Reset the roads distance values that changed in the last room generatio. This is a small optimisation to clear only the changed road g_cells.
	resetTouchedRoadGCells();
}

RG_Grower::~RG_Grower() {
	//void
}
