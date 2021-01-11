#pragma once

#include <base/gh.h>
#include "IGPGrid2DContent.h"
#include <worldGame3D/gw/gamepart/GamePart.h>

namespace rpg3D {
class GPG2DC_Base : public GamePart, virtual public IGPGrid2DContent {priv typedef GamePart super;pub dCtor(GPG2DC_Base);
    prot enum TrackingRegionType { None, GridBounds, CellsGrid };

    prot Vector2 gCellSize{};
    
    // This offset if from bottom left of gCell.
    prot Vector2 gridOriginOffset{0.0f, 0.0f};

    priv TrackingRegionType trackingRegion_type = None;
    // This is also initialised for CellsGrid and contains the grid lengths.
    priv Bounds2DInt trackingRegion_gridBounds{};
    priv sp<Array2D<unsigned char>> trackingRegion_cellsGrid = nullptr;

    prot bool allowMultipleEntitiesPerGCell;
    prot bool allowEntitiesOutsideOfTheGridArea;

    pub explicit GPG2DC_Base(
        base::IGameWorld* gw, 
        Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea
    );

    pub void initTrackingRegion_gridBounds(Bounds2DInt& trackingRegion_gridBounds) final;
    pub void initTrackingRegion_cellsGrid(sp<Array2D<unsigned char>> trackingRegion_cells) final;

    pub const Vector2& getGridOriginOffset() final;
    pub void setGridOriginOffset(Vector2& gridOriginOffset) final;

    pub void gPosToWorldPos(
        Vector2Int& gPos, float anchorX, float anchorY,
        Vector2& oWorldPos) final;
    pub void gPosToWorldPos(
        Vector2Int& gPos, float anchorX, float anchorY,
        Vector3& oWorldPos) final;
    pub void gPosCenterToWorldPos(Vector2Int& gPos, Vector2& oWorldPos) final;
    pub void worldPosToGPos(Vector2& worldPos, Vector2Int& oGPos) final;
    pub void worldPosToGPos(Vector3& worldPos, Vector2Int& oGPos) final;
    pub void worldPosToGCellCenterWorldPos(Vector2& worldPos) final;

    pub bool isGPosInTrackingRegion(Vector2Int& gPos) final;
    pub bool isGPosInTrackingRegion(int gX, int gY) final;

    prot Bounds2DInt& getGridTrackingRegionBounds();

    pub ~GPG2DC_Base() override;
};
};
