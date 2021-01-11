#pragma once

#include <base/gh.h>
#include "LZG_Base.h"
#include <base/list/Array2D.h>
#include <base/list/Array1D.h>
#include "util/ZoneGridCell.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class LZG_Grid : public LZG_Base {priv typedef LZG_Base super;pub dCtor(LZG_Grid);
    prot class SpecialLocation {pub dCtor(SpecialLocation);
        pub Vector2Int pos;

        pub bool isRoadConnected = false;

        pub SpecialLocation()
            : pos()
        {
            //void
        }

        pub SpecialLocation(Vector2Int& pos)
            : pos(pos)
        {
            //void
        }

        pub ~SpecialLocation() {};
    };

    priv static const int ZONE_GRID_PADDING;

    prot Array2D<ZoneGridCell>* zoneGrid = nullptr;

    prot Array1D<SpecialLocation>* specialLocationsList = nullptr;

    pub explicit LZG_Grid(
        std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
        Random* rng);

    prot void buildSpecialLocationsListAndZoneGrid();
    priv static void PrepareFragmentSpaceSpecialLocationsList(
        Array1D<Vector2Int>& fragmentSpaceSpecialLocationsList, int fragmentSpaceGridSizeX, int fragmentSpaceGridSizeY,
        Random* rng);
    priv void populateSpecialLocationsList(
        int fragmentSpaceGridCellSizeX, int fragmentSpaceGridCellSizeY, Array1D<Vector2Int>& fragmentSpaceSpecialLocationsList,
        int zoneGridPadding);

    prot void logIntermediaryPathingGrid();
    prot void logPathingRoadsGrid();
    prot void logPathingRoadsAndRoomsGrid();
    prot void logSpecialLocationsAndRoadsGrid();
    prot void logRooms();
    prot void logZoneGrid(bool drawPerZoneBgFill=false, bool drawRoadsWithPriority=false);

    pub ~LZG_Grid() override;
};
};
};
