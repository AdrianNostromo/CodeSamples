#pragma once

#include <base/gh.h>
#include "LZG_Grid.h"
#include <base/list/ArrayList.h>

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class LZG_Roads : public LZG_Grid {priv typedef LZG_Grid super;pub dCtor(LZG_Roads);
	prot ArrayList<Vector2Int>* roadGPositionsList = nullptr;

    pub explicit LZG_Roads(
        std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
		Random* rng);

	prot void buildRoads();

	priv static bool IsRoadsBuildFinished(Array1D<SpecialLocation>* specialLocationsList);
	priv static void InitCurrentPathingAlgorithmLocations(
		ArrayList<Vector2Int>& roadGPositionsList,
		ArrayList<Vector2Int>& currentPathAlgorithmLocations);
	priv bool managedTickPathingGeneratorAlgorithm(
		ArrayList<Vector2Int>& currentPathAlgorithmLocations,
		ArrayList<Vector2Int>& nextPathAlgorithmLocations);

	priv void reverseConnectPathedGridSlotToRoads(
		Vector2Int& startPos);
	priv static inline void AddRoadDirection(ZoneGridCell& gCell, Vector2Int& cPos, Vector2Int& directionNeighbourPos);
	priv static SpecialLocation& GetSpecialLocationAtGPos(Array1D<SpecialLocation>& specialLocationsList, int gPosX, int gPosY);

	prot void clearExistingRoadDistance(int pathingDistIndex);

	prot ArrayList<Vector2Int>* getRoadGPositionsList();

    pub ~LZG_Roads() override;
};
};
};
