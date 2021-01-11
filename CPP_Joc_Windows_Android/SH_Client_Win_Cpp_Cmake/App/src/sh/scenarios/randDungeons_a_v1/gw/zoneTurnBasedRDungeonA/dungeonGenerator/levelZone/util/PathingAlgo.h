#pragma once

#include <base/gh.h>
#include <functional>
#include <base/list/Array2D.h>
#include <base/list/ArrayList.h>
#include "ZoneGridCell.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class PathingAlgo {pub dCtor(PathingAlgo);
    priv bool processExistingRoadsOnly;
    priv bool pathingDistIndex;
    
    priv std::function<bool(Vector2Int & neighbourPos, ZoneGridCell & neighbourGCell)> cb_onNewPathGCell;

    priv Array2D<ZoneGridCell>* zoneGrid;
    priv ArrayList<Vector2Int>* roadGPositionsList;

    priv std::shared_ptr<ArrayList<Vector2Int>> nextPathAlgorithmLocations = nullptr;

    pub explicit PathingAlgo(
        bool processExistingRoadsOnly, bool pathingDistIndex,
        std::function<bool(Vector2Int& neighbourPos, ZoneGridCell& neighbourGCell)> cb_onNewPathGCell,
        Array2D<ZoneGridCell>* zoneGrid, ArrayList<Vector2Int>* roadGPositionsList);

    pub void init(Vector2Int& startPos);
    pub void tickPathing();

    pub bool isInProgress();
    pub ArrayList<Vector2Int>* peekNextPathAlgorithmLocations();

    pub virtual ~PathingAlgo();
};
};
};
