#pragma once

#include <base/gh.h>
#include "Drawable3DCreateConfigBase.h"
#include <base/list/ArrayList.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include <base/list/Array3D.h>

class Drawable3DCreateConfig_VoxelGrid : public Drawable3DCreateConfigBase {
    // This is a multiplier applied to the individual voxels, not their holder.
    pub float voxelSize;

    //asdAAAA;// use sp or use a copy of the data or leave as it is???;
    // Grid offset doesn't have the voxel size applied (1 == 1 grid slot size, it will be scaled after).
    pub Vector3* voxelGridOffset;

    // If this is null, it must be present as extra_data or as tiling_information.
    pub Array3D<signed char>* voxelGrid;
    pub std::string* voxelGridExtraDataIndicator;

    // If this is used without indexedVertexColor, this will
    pub ArrayList<Visual3DModelMaterial*>* materialsList;
    pub std::string* materialsListExtraDataIndicator;

    pub explicit Drawable3DCreateConfig_VoxelGrid(
        std::string id, std::string* parentId,
        Vector3* pos, Quaternion* rot, Vector3* scale,
        float voxelSize, Vector3* voxelGridOffset,
        Array3D<signed char>* voxelGrid, std::string* voxelGridExtraDataIndicator,
        ArrayList<Visual3DModelMaterial*>* materialsList, std::string* materialsListExtraDataIndicator
    );

    pub ~Drawable3DCreateConfig_VoxelGrid() override;
};
