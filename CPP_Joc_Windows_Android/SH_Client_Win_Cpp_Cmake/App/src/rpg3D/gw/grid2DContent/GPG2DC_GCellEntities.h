#pragma once

#include <base/gh.h>
#include "GPG2DC_Entities.h"
#include <base/list/ListDL.h>
#include <base/MM.h>
#include <base/map/Map2D.h>

namespace rpg3D {
class GPG2DC_GCellEntities : public GPG2DC_Entities {priv typedef GPG2DC_Entities super;pub dCtor(GPG2DC_GCellEntities);
    // There should be no empty list entries, delete the empty list entries to avoid large map.
    priv Map2D<int, int, sp<ListDL<IGrid2DContentModule*>>> gridEntitiesMap{};

    pub explicit GPG2DC_GCellEntities(
        base::IGameWorld* gw, 
        Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea
    );

    prot void onGridEntityHooked(IGrid2DContentModule* grid2DContent, Vector3& pos) override;
    prot void onGridEntityReleasePre(IGrid2DContentModule* grid2DContent) override;

    pub void gridEntityPosChanged(IGrid2DContentModule* grid2DContent, Vector3& pos) final;

    pub sp<ListDL<IGrid2DContentModule*>> getAllEntitiesAtGPos(int gPosX, int gPosY, bool errorOnOutOfBounds = true) final;
    
    pub bool getIsGCellOccupied(int gPosX, int gPosY) final;

    priv void addEntityToGCell(IGrid2DContentModule* grid2DContent, Vector3& pos);
    priv void removeEntityFromGCell(IGrid2DContentModule* grid2DContent);

    pub ~GPG2DC_GCellEntities() override;
};
};
