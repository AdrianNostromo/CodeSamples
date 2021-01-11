#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <base/math/Vector3.h>
#include <base/math/Vector2.h>
#include <base/math/Vector2Int.h>
#include <base/math/Bounds2DInt.h>
#include <base/list/ListDL.h>
#include <base/sp.h>
#include <base/list/Array2D.h>

namespace rpg3D {
    class IGrid2DContentModule;
};

namespace rpg3D {
class IGPGrid2DContent : virtual public IGamePart {priv typedef IGamePart super;
    pub static int TYPE;
    DEC_componentBasics();
    
    pub virtual void initTrackingRegion_gridBounds(Bounds2DInt& trackingRegion_gridBounds) = 0;
    pub virtual void initTrackingRegion_cellsGrid(sp<Array2D<unsigned char>> trackingRegion_cellsGrid) = 0;

    pub virtual const Vector2& getGridOriginOffset() = 0;
    pub virtual void setGridOriginOffset(Vector2& gridOriginOffset) = 0;

    pub virtual void gPosToWorldPos(
        Vector2Int& gPos, float anchorX, float anchorY,
        Vector2& oWorldPos) = 0;
    pub virtual void gPosToWorldPos(
        Vector2Int& gPos, float anchorX, float anchorY,
        Vector3& oWorldPos) = 0;
    pub virtual void gPosCenterToWorldPos(Vector2Int& gPos, Vector2& oWorldPos) = 0;
    pub virtual void worldPosToGPos(Vector2& worldPos, Vector2Int& oGPos) = 0;
    pub virtual void worldPosToGPos(Vector3& worldPos, Vector2Int& oGPos) = 0;

    pub virtual void worldPosToGCellCenterWorldPos(Vector2& worldPos) = 0;

    pub virtual bool isGPosInTrackingRegion(Vector2Int& gPos) = 0;
    pub virtual bool isGPosInTrackingRegion(int gX, int gY) = 0;

    pub virtual void hookGridEntity(IGrid2DContentModule* grid2DContent, Vector3& pos) = 0;
    pub virtual void releaseGridEntity(IGrid2DContentModule* grid2DContent) = 0;
    pub virtual void gridEntityPosChanged(IGrid2DContentModule* grid2DContent, Vector3& pos) = 0;

    pub virtual sp<ListDL<IGrid2DContentModule*>> getAllEntitiesAtGPos(int gPosX, int gPosY, bool errorOnOutOfBounds = true) = 0;

    pub virtual bool getIsGCellOccupied(int gPosX, int gPosY) = 0;

    pub virtual void hidePathableGCellsIndicator() = 0;
    // drawRadius; Use -1 to draw the entire grid.
    pub virtual void showPathableGCellsIndicator(Vector2Int& startGPos, int drawRadius, bool showStartGPosAsValidDestination) = 0;

    pub ~IGPGrid2DContent() override = default;
};
};
