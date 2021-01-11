#pragma once

#include <base/gh.h>
#include "RG_Pathing.h"
#include <base/math/Bounds2DInt.h>
#include <base/list/ArrayList.h>

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class RG_AlgoRegionRoomsFinder : public RG_Pathing {priv typedef RG_Pathing super;pub dCtor(RG_AlgoRegionRoomsFinder);
    priv Bounds2DInt roomsFinderAlgoZoneBounds{};

    // Distances available horizontally and vertically for each grid cell entry.
    priv std::unique_ptr<Array2D<Vector2Int>> roomWallDistancesGrid = nullptr;
    priv std::unique_ptr<Array1D<signed char>> tmp_roomRoadsValidationList = nullptr;

    // This is used as a optimisation to rooms-check only the entrie that are affected by the extra roads.
    priv Bounds2DInt roomGCellsToProcessBounds{};
    priv std::unique_ptr<Array2D<signed char>> roomGCellsToProcessMask = nullptr;

    pub explicit RG_AlgoRegionRoomsFinder();
    
    prot void initRoomWallDistancesGrid();
    priv void updateRoomWallDistancesGridForNewPathedRoad(Vector2Int& zoneRoadPos);
    
    // This is a special optimised algo that generates a fast list of all possible rooms in the algo region using optimised checks.
    // Returns the number of road gCells processed. Returns 0 when finished.
    // THe params optionalRoomSizeSmallAxisMax, optionalRoomSizeBigAxisMax are used only if their size is >0;
    prot int tickRoomsFinderAlgo(
        int& zoneRoomCandidatesDistanceFromOptimal, ArrayList<Bounds2DInt>& zoneRoomCandidatesList);
    // Returns the number of road gCells processed. Returns 0 when finished.
    priv int prepareGCellsToProcessRegion();
    // newRoadZoneGCellPos must not be a different room.
    priv void computMaskPartBoundsFromNewRoad(Vector2Int& newRoadZoneGCellPos, Bounds2DInt& oMaskPartRoomBounds);
    priv void findAndPushValidAllRoomsVariations(
        int srcRoomX, int srcRoomY,
        int& zoneRoomCandidatesDistanceFromOptimal, ArrayList<Bounds2DInt>& zoneRoomCandidatesList);
    priv bool validateRoomRoads(Bounds2DInt& roomZoneBounds);

    pub void clear() override;

    priv void zonePosToRoomAlgoSubZonePos(Vector2Int& zonePos, Vector2Int& algoSubZonePos);
    priv void zonePosToRoomAlgoSubZonePos(int zonePosX, int zonePosY, Vector2Int& algoSubZonePos);
    priv void roomAlgoSubZonePosToZonePos(Vector2Int& algoSubZonePos, Vector2Int& zonePos);
    priv void roomAlgoSubZonePosToZonePos(int algoSubZonePosX, int algoSubZonePosY, Vector2Int& zonePos);

    prot void logDistancesGrid();

    pub ~RG_AlgoRegionRoomsFinder() override;
};
};
};
