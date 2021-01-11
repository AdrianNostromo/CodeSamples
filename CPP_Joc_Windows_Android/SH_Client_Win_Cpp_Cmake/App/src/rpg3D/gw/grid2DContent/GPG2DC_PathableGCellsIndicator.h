#pragma once

#include <base/gh.h>
#include "GPG2DC_GCellReservations.h"
#include <base/list/ListDL.h>
#include <base/map/Map2D.h>
#include <memory>

namespace base {
    class WrappedWorldEntity;
};

namespace rpg3D {
class GPG2DC_PathableGCellsIndicator : public GPG2DC_GCellReservations {priv typedef GPG2DC_GCellReservations super;pub dCtor(GPG2DC_PathableGCellsIndicator);
    priv std::shared_ptr<base::WrappedWorldEntity> indicatorWEntity = nullptr;

    pub explicit GPG2DC_PathableGCellsIndicator(
        base::IGameWorld* gw, 
        Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea
    );

    pub void hidePathableGCellsIndicator() final;
    pub void showPathableGCellsIndicator(Vector2Int& startGPos, int drawRadius, bool showStartGPosAsValidDestination) final;

    pub ~GPG2DC_PathableGCellsIndicator() override;
};
};
