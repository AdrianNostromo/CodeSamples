#include "RG_Pathing.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

RG_Pathing::RG_Pathing()
	: super()
{
	currentPathingGCells = std::make_shared<ArrayList<Vector2Int>>();
	nextPathingGCells = std::make_shared<ArrayList<Vector2Int>>();
}

void RG_Pathing::clear() {
	super::clear();

	currentPathingGCells->clear();
	nextPathingGCells->clear();
}

std::shared_ptr<ArrayList<Vector2Int>> RG_Pathing::peekCurrentPathingGCells() {
	return currentPathingGCells;
}

int RG_Pathing::getCurrentPathingRoadDist() {
	return currentPathingRoadDist;
}

void RG_Pathing::initPathingAlgo(int roomIndex, Vector2Int& startPos) {
	zoneGrid->getReference(startPos.x, startPos.y).roomIndex = roomIndex;
	zoneGrid->getReference(startPos.x, startPos.y).pathingDists[0] = 0; 
	touchedRoadGCells.append_emplace(startPos.x, startPos.y);
	
	currentPathingRoadDist = 0;
	currentPathingGCells->append_emplace(startPos);
}

bool RG_Pathing::tickPathingAlgo() {
	// Path only the roads using the road_dir_info.

	currentPathingRoadDist++;

	nextPathingGCells->clear();

	for (int i = 0; i < currentPathingGCells->count(); i++) {
		Vector2Int& cPos = currentPathingGCells->getReference(i);
		ZoneGridCell& cGCell = zoneGrid->getReference(cPos.x, cPos.y);
		if (cGCell.pathingDists[0] != currentPathingRoadDist - 1) {
			throw LogicException(LOC);
		}

		// Check all neighbours and do logic. The locations are from the road directions.
		for (int j = 0; j < 4; j++) {
			ZoneGridCell::DirInfo& pNeighbourInfo = ZoneGridCell::directionsInfo.getReference(j);

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

			if (neighbourGCell.roadDirBitMap != 0 && neighbourGCell.pathingDists[0] == -1) {
				// Road or special_location.

				nextPathingGCells->append_emplace(neighbourPos.x, neighbourPos.y);
				neighbourGCell.pathingDists[0] = currentPathingRoadDist;
				touchedRoadGCells.append_emplace(neighbourPos.x, neighbourPos.y);
			}
		}
	}

	// Invert the lists.
	std::shared_ptr<ArrayList<Vector2Int>> t = currentPathingGCells;
	currentPathingGCells = nextPathingGCells;
	nextPathingGCells = t;

	if (currentPathingGCells->count() == 0) {
		return false;
	}

	return true;
}

void RG_Pathing::resetTouchedRoadGCells() {
	if (touchedRoadGCells.count() > 0) {
		for (int i = 0; i < touchedRoadGCells.count(); i++) {
			Vector2Int& p = touchedRoadGCells.getReference(i);
			zoneGrid->getReference(p.x, p.y).pathingDists[0] = -1;
		}
		touchedRoadGCells.clear();
	}
}

RG_Pathing::~RG_Pathing() {
	//void
}
