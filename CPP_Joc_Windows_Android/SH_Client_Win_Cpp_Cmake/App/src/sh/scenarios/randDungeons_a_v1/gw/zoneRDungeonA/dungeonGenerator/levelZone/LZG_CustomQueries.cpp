#include "LZG_CustomQueries.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

LZG_CustomQueries::LZG_CustomQueries(
	std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
	Random* rng)
	: super(
		config, zoneIndex,
		rng)
{
	//void
}

void LZG_CustomQueries::findInOutLocations(
	SpecialLocationsDistPair& oSpecialLocationsDistPair)
{
	// Start at each location and compute the farthest special_locations road.
	// The longest road makes the start and end special locations.
	// Start at each special location and A* find the farthest special location.
	for (int roomIndex = 0; roomIndex < specialLocationsList->getCount(); roomIndex++) {
		SpecialLocation& pgSource = specialLocationsList->getReference(roomIndex);

		computeFarthestSpecialLocationAStarAdditiveOutput(
			pgSource.pos, roomIndex, 0/*pathingDistIndex*/,
			oSpecialLocationsDistPair
		);
	}
}

void LZG_CustomQueries::computeFarthestSpecialLocationAStarAdditiveOutput(
	Vector2Int& startGPos, int startRoomIndex, int pathingDistIndex,
	SpecialLocationsDistPair& oSpecialLocationsDistPair)
{
	// Algo needs -1 for roads distances.
	clearExistingRoadDistance(pathingDistIndex);

	ArrayList<Vector2Int> aPathAlgorithmLocations{};
	ArrayList<Vector2Int> bPathAlgorithmLocations{};
	bool isCachesSwapped = false;

	aPathAlgorithmLocations.append_emplace(startGPos);
	zoneGrid->getReference(startGPos.x, startGPos.y).pathingDists[pathingDistIndex] = 0;

	int totalSpecialLocationsDetectedCounter = 0;
	while (true) {
		// Process all actives list.
		bool isFinished = ManagedTickDistanceFindPathingAlgorithm(
			zoneGrid, pathingDistIndex, totalSpecialLocationsDetectedCounter,
			!isCachesSwapped ? aPathAlgorithmLocations : bPathAlgorithmLocations,
			!isCachesSwapped ? bPathAlgorithmLocations : aPathAlgorithmLocations,
			startRoomIndex, oSpecialLocationsDistPair
		);
		isCachesSwapped = !isCachesSwapped;

		if (isFinished) {
			if (totalSpecialLocationsDetectedCounter != specialLocationsList->getCount() - 1) {
				// Not all locations were found.
				throw LogicException(LOC);
			}

			break;
		}
	}
}

// return isFinished::bool;
bool LZG_CustomQueries::ManagedTickDistanceFindPathingAlgorithm(
	Array2D<ZoneGridCell>* zoneGrid, int pathingDistIndex, int& totalSpecialLocationsDetectedCounter,
	ArrayList<Vector2Int>& currentPathAlgorithmLocations,
	ArrayList<Vector2Int>& nextPathAlgorithmLocations,
	int startRoomIndex, SpecialLocationsDistPair& oSpecialLocationsDistPair)
{
	nextPathAlgorithmLocations.clear();

	for (int i = 0; i < currentPathAlgorithmLocations.count(); i++) {
		Vector2Int& srcPos = currentPathAlgorithmLocations.getReference(i);

		ZoneGridCell& srcVal = zoneGrid->getReference(srcPos.x, srcPos.y);

		// Check all neighbours and do logic.
		for (int j = 0; j < 4; j++) {
			ZoneGridCell::DirInfo& pNeighbourInfo = ZoneGridCell::directionsInfo.getReference(j);

			if ((srcVal.roadDirBitMap & pNeighbourInfo.roadDir) == 0) {
				// The current grid cell road doesn't go in the direction of this neighbour.
				continue;
			}

			Vector2Int neighbourPos{ srcPos.x + pNeighbourInfo.neighbourOffset.x, srcPos.y + pNeighbourInfo.neighbourOffset.y };

			if (neighbourPos.x < 0 || neighbourPos.x >= zoneGrid->getLengthA()
				|| neighbourPos.y < 0 || neighbourPos.y >= zoneGrid->getLengthB())
			{
				continue;
			}

			ZoneGridCell& valNeighbour = zoneGrid->getReference(neighbourPos.x, neighbourPos.y);

			if (valNeighbour.roadDirBitMap != 0 && valNeighbour.pathingDists[pathingDistIndex] == -1) {
				// Road or special_location.

				nextPathAlgorithmLocations.append_emplace(neighbourPos.x, neighbourPos.y);
				valNeighbour.pathingDists[pathingDistIndex] = srcVal.pathingDists[pathingDistIndex] + 1;

				if (valNeighbour.roomIndex >= 0) {
					totalSpecialLocationsDetectedCounter++;

					// The dstance is always the shortest one (in the case of multiple paths) because each gCell is processed only once. The logic is correct.
					// A special location detected, check the distance.
					if (oSpecialLocationsDistPair.longestLocationsRoad < 0 || valNeighbour.pathingDists[pathingDistIndex] > oSpecialLocationsDistPair.longestLocationsRoad) {
						oSpecialLocationsDistPair.set(
							startRoomIndex/*locationIndexA*/, valNeighbour .roomIndex/*locationIndexA*/,
							valNeighbour.pathingDists[pathingDistIndex]/*longestLocationsRoad*/
						);
					}
				}
			}
		}
	}

	if (nextPathAlgorithmLocations.count() > 0) {
		return false;
	} else {
		return true;
	}
}

LZG_CustomQueries::~LZG_CustomQueries() {
	//void
}
