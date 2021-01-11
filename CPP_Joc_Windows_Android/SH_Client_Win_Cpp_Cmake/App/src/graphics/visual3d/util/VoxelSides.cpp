#include "VoxelSides.h"
#include <base/math/util/BoolUtil.h>

const int VoxelSides::xMin = 0;
const int VoxelSides::xMax = 1;

const int VoxelSides::yMin = 2;
const int VoxelSides::yMax = 3;

const int VoxelSides::zMin = 4;
const int VoxelSides::zMax = 5;

const int VoxelSides::BitGroup_xMin = BoolUtil::setBitAtIndexDirect(0, xMin);
const int VoxelSides::BitGroup_xMax = BoolUtil::setBitAtIndexDirect(0, xMax);

const int VoxelSides::BitGroup_yMin = BoolUtil::setBitAtIndexDirect(0, yMin);
const int VoxelSides::BitGroup_yMax = BoolUtil::setBitAtIndexDirect(0, yMax);

const int VoxelSides::BitGroup_zMin = BoolUtil::setBitAtIndexDirect(0, zMin);
const int VoxelSides::BitGroup_zMax = BoolUtil::setBitAtIndexDirect(0, zMax);

const int VoxelSides::Front = yMin;
const int VoxelSides::Back = yMax;

const int VoxelSides::Left = xMin;
const int VoxelSides::Right = xMax;

const int VoxelSides::Up = zMax;
const int VoxelSides::Down = zMin;
