#pragma once

#include <base/gh.h>
#include "GPG2DC_GCellEntities.h"
#include <base/list/ListDL.h>
#include <base/map/Map2D.h>
#include <base/MM.h>

namespace rpg3D {
class GPG2DC_GCellReservations : public GPG2DC_GCellEntities {priv typedef GPG2DC_GCellEntities super;pub dCtor(GPG2DC_GCellReservations);
   
//asd_01;// implement this, it is used by the sqaure tactics arena movement.

    pub explicit GPG2DC_GCellReservations(
        base::IGameWorld* gw, 
        Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea
    );

    pub ~GPG2DC_GCellReservations() override;
};
};
