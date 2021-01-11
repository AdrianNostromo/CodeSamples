#pragma once

#include <base/gh.h>
#include "RG_Base.h"
#include <base/list/ArrayList.h>

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class RG_Pathing : public RG_Base {priv typedef RG_Base super;pub dCtor(RG_Pathing);
    // The start position has a dist of 0.
    priv int currentPathingRoadDist = -1;
    priv std::shared_ptr<ArrayList<Vector2Int>> currentPathingGCells;
    priv std::shared_ptr<ArrayList<Vector2Int>> nextPathingGCells;
    
    // This is used to optimise the cleanup when the algo finishes. This is used to not clear the entire roads grid when the next room will be grown.
    priv ArrayList<Vector2Int> touchedRoadGCells{};

    pub explicit RG_Pathing();

    pub void clear() override;

    prot std::shared_ptr<ArrayList<Vector2Int>> peekCurrentPathingGCells();
    prot int getCurrentPathingRoadDist();

    prot void initPathingAlgo(int roomIndex, Vector2Int& startPos);

    // Returns the true if there are road g_cells to process in the next tick.
    // This returns false also if all next pathing g_locations are outside of the algo region.
    prot bool tickPathingAlgo();

    prot void resetTouchedRoadGCells();

    pub ~RG_Pathing() override;
};
};
};
