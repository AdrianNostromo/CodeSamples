#include "Drawable3DCreateConfig_VoxelGrid.h"

Drawable3DCreateConfig_VoxelGrid::Drawable3DCreateConfig_VoxelGrid(
    std::string id, std::string* parentId,
    Vector3* pos, Quaternion* rot, Vector3* scale,
    float voxelSize, Vector3* voxelGridOffset,
    Array3D<signed char>* voxelGrid, std::string* voxelGridExtraDataIndicator,
    ArrayList<Visual3DModelMaterial*>* materialsList, std::string* materialsListExtraDataIndicator
)
    :Drawable3DCreateConfigBase(id, parentId, pos, rot, scale),
     voxelSize(voxelSize), voxelGridOffset(voxelGridOffset),
     voxelGrid(voxelGrid), voxelGridExtraDataIndicator(voxelGridExtraDataIndicator),
     materialsList(materialsList), materialsListExtraDataIndicator(materialsListExtraDataIndicator)
{
    //void
}

Drawable3DCreateConfig_VoxelGrid::~Drawable3DCreateConfig_VoxelGrid() {
    //void
}
