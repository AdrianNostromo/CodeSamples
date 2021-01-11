#pragma once

#include <base/gh.h>
#include "TilingInformation.h"
#include <base/list/ArrayList.h>
#include <base/list/Array2D.h>
#include <base/math/Vector3.h>
#include <base/sp.h>

namespace graphics {
class TilingInformation_GridXY : public TilingInformation {priv typedef TilingInformation super;pub dCtor(TilingInformation_GridXY);
    pub static const int TYPE;
    
    // This may have nullptr grids that will not draw visuals but will create physics.
    pub std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList;
    // <0;// Draw -1 in the voxels grid and no physics created.
    // >=0&&nullptr grid value; // Draw -1 in the voxels and physics is created.
    pub std::shared_ptr<Array2D<signed char>> tilingGrid;

    // This is used only when loading saved level data and not when creating hardcoded content (in that case it is auto-updated correctly.
    pub sp<Vector3> geometryGridOffset;

    pub explicit TilingInformation_GridXY(
        Vector3Int& tileVoxelsCount, int enabledSidesTriangulationBitMask,
        std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>>& tilesConfigList,
        std::shared_ptr<Array2D<signed char>>& tilingGrid, sp<Vector3> geometryGridOffset);

    pub ~TilingInformation_GridXY() override;
};
};
