#include "ZoneGridCell.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

const int ZoneGridCell::RoadDir::xPositive = 1;
const int ZoneGridCell::RoadDir::xNegative = 2;
const int ZoneGridCell::RoadDir::yPositive = 4;
const int ZoneGridCell::RoadDir::yNegative = 8;
const int ZoneGridCell::RoadDir::Placeholder = 16;

Array1D<ZoneGridCell::DirInfo> ZoneGridCell::directionsInfo{ 4, new DirInfo[4] {
    {ZoneGridCell::RoadDir::xPositive, Vector2Int{1, 0}},
    {ZoneGridCell::RoadDir::xNegative, Vector2Int{-1, 0}},
    {ZoneGridCell::RoadDir::yPositive, Vector2Int{0, 1}},
    {ZoneGridCell::RoadDir::yNegative, Vector2Int{0, -1}}
}};
