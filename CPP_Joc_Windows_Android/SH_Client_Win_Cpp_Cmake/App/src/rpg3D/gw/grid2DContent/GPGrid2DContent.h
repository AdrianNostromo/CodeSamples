#pragma once

#include <base/gh.h>
#include "GPG2DC_PathableGCellsIndicator.h"

namespace rpg3D {
class GPGrid2DContent : public GPG2DC_PathableGCellsIndicator {priv typedef GPG2DC_PathableGCellsIndicator super;pub dCtor(GPGrid2DContent);
    pub explicit GPGrid2DContent(
        base::IGameWorld* gw, 
        Vector2 gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea
    );

    pub ~GPGrid2DContent() override;
};
};
