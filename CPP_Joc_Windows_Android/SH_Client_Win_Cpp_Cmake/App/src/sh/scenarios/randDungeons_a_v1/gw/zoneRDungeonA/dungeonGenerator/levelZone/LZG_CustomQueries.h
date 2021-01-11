#pragma once

#include <base/gh.h>
#include "LZG_GridRomify.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class LZG_CustomQueries : public LZG_GridRomify {priv typedef LZG_GridRomify super;pub dCtor(LZG_CustomQueries);
    prot class SpecialLocationsDistPair {
		pub int locationIndexA = -1;
		pub int locationIndexB = -1;
		pub int longestLocationsRoad = -1;

		pub void set(int locationIndexA, int locationIndexB, int longestLocationsRoad) {
			this->locationIndexA = locationIndexA;
			this->locationIndexB = locationIndexB;
			this->longestLocationsRoad = longestLocationsRoad;
		}
	};
	
	pub explicit LZG_CustomQueries(
        std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
		Random* rng);

	prot void findInOutLocations(
		SpecialLocationsDistPair& oSpecialLocationsDistPair);
	priv void computeFarthestSpecialLocationAStarAdditiveOutput(
		Vector2Int& startGPos, int startRoomIndex, int pathingDistIndex,
		SpecialLocationsDistPair& oSpecialLocationsDistPair);
	// return isFinished::bool;
	priv static bool ManagedTickDistanceFindPathingAlgorithm(
		Array2D<ZoneGridCell>* zoneGrid, int pathingDistIndex, int& totalSpecialLocationsDetectedCounter,
		ArrayList<Vector2Int>& currentPathAlgorithmLocations,
		ArrayList<Vector2Int>& nextPathAlgorithmLocations,
		int startRoomIndex, SpecialLocationsDistPair& oSpecialLocationsDistPair);

    pub ~LZG_CustomQueries() override;
};
};
};
