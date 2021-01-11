#include "PathingAlgo.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

PathingAlgo::PathingAlgo(
	bool processExistingRoadsOnly, bool pathingDistIndex,
	std::function<bool(Vector2Int & neighbourPos, ZoneGridCell & neighbourGCell)> cb_onNewPathGCell,
	Array2D<ZoneGridCell>* zoneGrid, ArrayList<Vector2Int>* roadGPositionsList)
	: processExistingRoadsOnly(processExistingRoadsOnly), pathingDistIndex(pathingDistIndex),
	cb_onNewPathGCell(cb_onNewPathGCell), 
	zoneGrid(zoneGrid), roadGPositionsList(roadGPositionsList)
{
	//void
}

void PathingAlgo::init(Vector2Int& startPos) {
	nextPathAlgorithmLocations = std::make_shared<ArrayList<Vector2Int>>();
	
	// Clear the grid or roads distances to -1.
	if (processExistingRoadsOnly) {
		// Clear roads only.
		if (roadGPositionsList == nullptr) {
			throw LogicException(LOC);
		}

		for (int i = 0; i < roadGPositionsList->count(); i++) {
			zoneGrid->getReference(roadGPositionsList->getReference(i)).pathingDists[pathingDistIndex] = -1;
		}
	} else {
		// CLear entire grid.
		for (int gX = 0; gX < zoneGrid->getLengthA(); gX++) {
			for (int gY = 0; gY < zoneGrid->getLengthB(); gY++) {
				zoneGrid->getReference(gX, gY).pathingDists[pathingDistIndex] = -1;
			}
		}
	}

	// Set the origin road cost to 0;
	nextPathAlgorithmLocations->append_emplace(startPos);
	zoneGrid->getReference(startPos.x, startPos.y).pathingDists[pathingDistIndex] = 0;
}

void PathingAlgo::tickPathing() {
	std::shared_ptr<ArrayList<Vector2Int>> currentPathAlgorithmLocations = nextPathAlgorithmLocations;
	nextPathAlgorithmLocations = std::make_shared<ArrayList<Vector2Int>>();

	for (int i = 0; i < currentPathAlgorithmLocations->count(); i++) {
		Vector2Int& cPos = currentPathAlgorithmLocations->getReference(i);
		ZoneGridCell& cGCell = zoneGrid->getReference(cPos.x, cPos.y);

		// Check all neighbours and do logic.
		for (int directionIndex = 0; directionIndex < 4; directionIndex++) {
			ZoneGridCell::DirInfo& pNeighbourInfo = ZoneGridCell::directionsInfo.getReference(directionIndex);

			if ((cGCell.roadDirBitMap & pNeighbourInfo.roadDir) == 0) {
				// The current grid cell road doesn't go in the direction of this neighbour.
				continue;
			}

			Vector2Int neighbourPos{ cPos.x + pNeighbourInfo.neighbourOffset.x, cPos.y + pNeighbourInfo.neighbourOffset.y };

			if (neighbourPos.x < 0 || neighbourPos.x >= zoneGrid->getLengthA()
				|| neighbourPos.y < 0 || neighbourPos.y >= zoneGrid->getLengthB())
			{
				continue;
			}

			ZoneGridCell& neighbourGCell = zoneGrid->getReference(neighbourPos.x, neighbourPos.y);

			if (processExistingRoadsOnly) {
				if (neighbourGCell.roadDirBitMap != 0 && neighbourGCell.pathingDists[pathingDistIndex] == -1) {
					nextPathAlgorithmLocations->append_emplace(neighbourPos.x, neighbourPos.y);
					neighbourGCell.pathingDists[pathingDistIndex] = cGCell.pathingDists[pathingDistIndex] + 1;

					if (cb_onNewPathGCell) {
						bool isFinished = cb_onNewPathGCell(neighbourPos, neighbourGCell);
						if (isFinished) {
							// Logic finished.
							return;
						}
					}
				}
			} else {
				// Full grid processing is not implemented currently. Implement when needed.
				throw LogicException(LOC);
			}
		}
	}
}

bool PathingAlgo::isInProgress() {
	if (nextPathAlgorithmLocations == nullptr) {
		throw LogicException(LOC);
	}

	return nextPathAlgorithmLocations->count() > 0 ? true : false;
}

ArrayList<Vector2Int>* PathingAlgo::peekNextPathAlgorithmLocations() {
	if (nextPathAlgorithmLocations == nullptr) {
		throw LogicException(LOC);
	}

	return nextPathAlgorithmLocations.get();
}

PathingAlgo::~PathingAlgo() {
	//void
}
