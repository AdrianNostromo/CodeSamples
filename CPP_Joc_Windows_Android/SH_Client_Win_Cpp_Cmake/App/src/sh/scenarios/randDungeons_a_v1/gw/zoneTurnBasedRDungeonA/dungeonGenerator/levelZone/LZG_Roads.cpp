#include "LZG_Roads.h"
#include <iostream>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

LZG_Roads::LZG_Roads(
	std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
	Random* rng)
	: super(
		config, zoneIndex,
		rng)
{
	//void
}

void LZG_Roads::buildRoads() {
	roadGPositionsList = new ArrayList<Vector2Int>{ zoneGrid->getLengthA() * zoneGrid->getLengthB() };

	// While no all connected do pathing part.
	// The algorithm will connect all to 0 location.
	specialLocationsList->getReference(0).isRoadConnected = true;

	roadGPositionsList->append_emplace(specialLocationsList->getReference(0).pos);
	ZoneGridCell& t = zoneGrid->getReference(
		specialLocationsList->getReference(0).pos.x, specialLocationsList->getReference(0).pos.y
	);
	t.pathingDists[0] = 0;
	t.roadDirBitMap |= ZoneGridCell::RoadDir::Placeholder;

	ArrayList<Vector2Int> aPathAlgorithmLocations{ zoneGrid->getLengthA() * zoneGrid->getLengthB() };
	ArrayList<Vector2Int> bPathAlgorithmLocations{ zoneGrid->getLengthA() * zoneGrid->getLengthB() };
	
	ArrayList<Vector2Int>* currentPathAlgorithmLocations = &aPathAlgorithmLocations;
	ArrayList<Vector2Int>* nextPathAlgorithmLocations = &bPathAlgorithmLocations;

	while (true) {
		if (IsRoadsBuildFinished(specialLocationsList)) {
			// All are connected.
			break;
		}

		InitCurrentPathingAlgorithmLocations(
			*roadGPositionsList,
			*currentPathAlgorithmLocations
		);

		while (true) {
			nextPathAlgorithmLocations->clear();

			// Build a road from the current net to a unconnected slot.
			// Do a A* tick and watch for a new unconnected location.
			// On an unconnected location detection, do A* reverse connection until a value_1 grid cell is reached (making all passed through have a value 0);
			bool isNewLocationReached = managedTickPathingGeneratorAlgorithm(
				*currentPathAlgorithmLocations,
				*nextPathAlgorithmLocations
			);

			//logIntermediaryPathingGrid();

			if (isNewLocationReached) {
				break;
			}

			ArrayList<Vector2Int>* t = currentPathAlgorithmLocations;
			currentPathAlgorithmLocations = nextPathAlgorithmLocations;
			nextPathAlgorithmLocations = t;
		}
	}
}

bool LZG_Roads::IsRoadsBuildFinished(Array1D<SpecialLocation>* specialLocationsList) {
	for (int i = 0; i < specialLocationsList->getCount(); i++) {
		SpecialLocation& pgSource = specialLocationsList->getReference(i);

		if (!pgSource.isRoadConnected) {
			return false;
		}
	}

	return true;
}

void LZG_Roads::InitCurrentPathingAlgorithmLocations(
	ArrayList<Vector2Int>& roadGPositionsList,
	ArrayList<Vector2Int>& currentPathAlgorithmLocations)
{
	currentPathAlgorithmLocations.clear();
	for (int i = 0; i < roadGPositionsList.count(); i++) {
		currentPathAlgorithmLocations.append_emplace(roadGPositionsList.getReference(i));
	}
}

bool LZG_Roads::managedTickPathingGeneratorAlgorithm(
	ArrayList<Vector2Int>& currentPathAlgorithmLocations,
	ArrayList<Vector2Int>& nextPathAlgorithmLocations)
{
	nextPathAlgorithmLocations.clear();

	// Diagonals are included because they make the roads more random-ish (because of the game world nature of cubes).
	// Diagonals are also corner-filled so they will be pathble by units.
	static Vector2Int directionOffsets[8]{
		Vector2Int{-1, 0},
		Vector2Int{1, 0},
		Vector2Int{0, -1},
		Vector2Int{0, 1},

		Vector2Int{-1, -1},
		Vector2Int{-1, 1},
		Vector2Int{1, 1},
		Vector2Int{1, -1}
	};

	for (int i = 0; i < currentPathAlgorithmLocations.count(); i++) {
		Vector2Int& srcPos = currentPathAlgorithmLocations.getReference(i);

		ZoneGridCell& srcVal = zoneGrid->getReference(srcPos.x, srcPos.y);

		// Check all neighbours and do logic.
		for (int j = 0; j < 8; j++) {
			Vector2Int& pNeighbourOffset = directionOffsets[j];

			Vector2Int neighbourPos{ srcPos.x + pNeighbourOffset.x, srcPos.y + pNeighbourOffset.y };

			if (neighbourPos.x < 0 || neighbourPos.x >= zoneGrid->getLengthA()
				|| neighbourPos.y < 0 || neighbourPos.y >= zoneGrid->getLengthB())
			{
				continue;
			}

			ZoneGridCell& valNeighbour = zoneGrid->getReference(neighbourPos.x, neighbourPos.y);
			if (valNeighbour.roadDirBitMap != 0) {
				// Already a road, ignore because this function is expanding the only road that currently exists.

				//void
			} else if(valNeighbour.pathingDists[0] == -1 || srcVal.pathingDists[0] + 1 <= valNeighbour.pathingDists[0]) {
				valNeighbour.pathingDists[0] = srcVal.pathingDists[0] + 1;

				nextPathAlgorithmLocations.append_emplace(neighbourPos.x, neighbourPos.y);

				if (valNeighbour.roomIndex >= 0) {
					// A unconnected special location found.
					// Mark the location as connected.
					GetSpecialLocationAtGPos(*specialLocationsList, neighbourPos.x, neighbourPos.y)
						.isRoadConnected = true;

					reverseConnectPathedGridSlotToRoads(
						neighbourPos
					);

					return true;
				}
			}
		}
	}

	return false;
}

void LZG_Roads::reverseConnectPathedGridSlotToRoads(
	Vector2Int& startPos)
{
	// Diagonals are included because they make the roads more random-ish (because of the game world nature of cubes).
	// Diagonals are also corner-filled so they will be pathble by units.
	static Vector2Int directionOffsets[8]{
		Vector2Int{-1, 0},
		Vector2Int{1, 0},
		Vector2Int{0, -1},
		Vector2Int{0, 1},

		Vector2Int{-1, -1},
		Vector2Int{-1, 1},
		Vector2Int{1, 1},
		Vector2Int{1, -1}
	};

	Vector2Int cPos(startPos);

	ArrayList<Vector2Int> newRoadGridLocations{ zoneGrid->getReference(cPos.x, cPos.y).pathingDists[0] };

	ArrayList<Vector2Int> validNeighbourPositions{ 8 };

	//logIntermediaryPathingGrid();

	while (true) {
		bool isRoadNetReached = false;

		ZoneGridCell& cVal = zoneGrid->getReference(cPos.x, cPos.y);
		if (cVal.roadDirBitMap != 0 || cVal.pathingDists[0] <= 0) {
			// The value must be a A* >0 value that is not already a road.
			throw LogicException(LOC);
		}

		// Add current grid cell to the road.
		newRoadGridLocations.append_emplace(cPos);

		validNeighbourPositions.clear();
		// Find the next grid cell direction. Only check for lower value.
		for (int j = 0; j < 8; j++) {
			Vector2Int& pNeighbourOffset = directionOffsets[j];
			Vector2Int pNeighbourPos{ cPos.x + pNeighbourOffset.x, cPos.y + pNeighbourOffset.y };

			if (pNeighbourPos.x < 0 || pNeighbourPos.x >= zoneGrid->getLengthA()
				|| pNeighbourPos.y < 0 || pNeighbourPos.y >= zoneGrid->getLengthB())
			{
				continue;
			}

			ZoneGridCell& neighbourVal = zoneGrid->getReference(pNeighbourPos.x, pNeighbourPos.y);

			// Don't pick the first neighbour, save all valid neighbours in a list and pick one at random.
			if (neighbourVal.pathingDists[0] == cVal.pathingDists[0] - 1) {
				if (neighbourVal.roadDirBitMap != 0) {
					// Main roads reached.
					// Need to place diagonal corner if needed still.
					isRoadNetReached = true;

					newRoadGridLocations.append_emplace(pNeighbourPos);

					break;
				}

				// Finished.
				validNeighbourPositions.append_emplace(pNeighbourPos);
			}
		}
		if (isRoadNetReached) {
			break;
		}
		if (validNeighbourPositions.count() == 0) {
			throw LogicException(LOC);
		}

		// Chose a randmo neighbour path.
		int i1 = rng->nextInt(0, validNeighbourPositions.count() - 1);
		Vector2Int& finalNeighbourPos = validNeighbourPositions.getReference(i1);

		cPos.set(finalNeighbourPos);
	}

	cPos.set(newRoadGridLocations.getReference(0));
	for (int i = 1; i < newRoadGridLocations.count(); i++) {
		Vector2Int& neighbourPos = newRoadGridLocations.getReference(i);
		ZoneGridCell& neighbourGCell = zoneGrid->getReference(neighbourPos.x, neighbourPos.y);

		//logIntermediaryPathingGrid();

		// The last entry is a road entry already.
		ZoneGridCell& gCell = zoneGrid->getReference(cPos.x, cPos.y);
		gCell.roadDirBitMap |= ZoneGridCell::RoadDir::Placeholder;
		gCell.pathingDists[0] = 0;

		Vector2Int finalNeighbourPos{};

		roadGPositionsList->append_emplace(cPos.x, cPos.y);

		//logIntermediaryPathingGrid();

		if (cPos.x != neighbourPos.x && cPos.y != neighbourPos.y) {
			// Diagonal.
			if (zoneGrid->getReference(cPos.x, neighbourPos.y).roadDirBitMap == 0 && zoneGrid->getReference(neighbourPos.x, cPos.y).roadDirBitMap == 0) {
				// Randomly chose a non-road corner.
				// Pure diagonal (no corners). Need to add a corner.
				if (rng->nextFloat() <= 0.5f) {
					// Add a left corner.
					finalNeighbourPos.set(cPos.x, neighbourPos.y);
				} else {
					// Add a right corent.
					finalNeighbourPos.set(neighbourPos.x, cPos.y);
				}
			} else if (zoneGrid->getReference(cPos.x, neighbourPos.y).roadDirBitMap == 0) {
				// Left corner is unused, use it.
				finalNeighbourPos.set(cPos.x, neighbourPos.y);
			} else if (zoneGrid->getReference(neighbourPos.x, cPos.y).roadDirBitMap == 0) {
				// Right corner is unused, use it.
				finalNeighbourPos.set(neighbourPos.x, cPos.y);
			} else {
				// Both corners are used, error. This case probably never occurs but not sure. Reason this never occurs is because only 1 road is connected to the roads_net at a time.
				throw LogicException(LOC);
			}

			ZoneGridCell& cornerGCell = zoneGrid->getReference(finalNeighbourPos.x, finalNeighbourPos.y);
			cornerGCell.roadDirBitMap |= ZoneGridCell::RoadDir::Placeholder;
			cornerGCell.pathingDists[0] = 0;
				
			// Add forward direction road.
			AddRoadDirection(cornerGCell, finalNeighbourPos, neighbourPos);
			AddRoadDirection(neighbourGCell, neighbourPos, finalNeighbourPos);

			roadGPositionsList->append_emplace(finalNeighbourPos.x, finalNeighbourPos.y);
		} else {
			finalNeighbourPos.set(neighbourPos);
		}

		AddRoadDirection(gCell, cPos, finalNeighbourPos);
		ZoneGridCell& finalNeighbourGCell = zoneGrid->getReference(finalNeighbourPos.x, finalNeighbourPos.y);
		AddRoadDirection(finalNeighbourGCell, finalNeighbourPos, cPos);

		//logIntermediaryPathingGrid();

		cPos.set(neighbourPos);
	}
}

inline void LZG_Roads::AddRoadDirection(ZoneGridCell& gCell, Vector2Int& cPos, Vector2Int& directionNeighbourPos) {
	//std::cout << cPos.x << "/" << cPos.y << " " << directionNeighbourPos.x << "/" << directionNeighbourPos.y;
	if (cPos.y == directionNeighbourPos.y) {
		if (cPos.x < directionNeighbourPos.x) {
			//std::cout << " xPositive";

			gCell.roadDirBitMap |= ZoneGridCell::RoadDir::xPositive;
		} else {
			//std::cout << " xNegative";

			gCell.roadDirBitMap |= ZoneGridCell::RoadDir::xNegative;
		}
	} else if (cPos.x == directionNeighbourPos.x) {
		if (cPos.y < directionNeighbourPos.y) {
			//std::cout << " yPositive";

			gCell.roadDirBitMap |= ZoneGridCell::RoadDir::yPositive;
		} else {
			//std::cout << " yNegative";

			gCell.roadDirBitMap |= ZoneGridCell::RoadDir::yNegative;
		}
	}
	//std::cout << std::endl;
}

LZG_Roads::SpecialLocation& LZG_Roads::GetSpecialLocationAtGPos(Array1D<SpecialLocation>& specialLocationsList, int gPosX, int gPosY) {
	for (int i = 0; i < specialLocationsList.getCount(); i++) {
		SpecialLocation& pgSource = specialLocationsList.getReference(i);

		if (pgSource.pos.equals(gPosX, gPosY)) {
			return pgSource;
		}
	}

	throw LogicException(LOC);
}

void LZG_Roads::clearExistingRoadDistance(int pathingDistIndex) {
	for (int i = 0; i < roadGPositionsList->count(); i++) {
		Vector2Int& p = roadGPositionsList->getReference(i);

		zoneGrid->getReference(p.x, p.y).pathingDists[pathingDistIndex] = -1;
	}
}

ArrayList<Vector2Int>* LZG_Roads::getRoadGPositionsList() {
	return roadGPositionsList;
}

LZG_Roads::~LZG_Roads() {
	if (roadGPositionsList != nullptr) {
		delete roadGPositionsList;
		roadGPositionsList = nullptr;
	}
}
