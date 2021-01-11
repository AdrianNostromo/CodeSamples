#pragma once

#include <base/gh.h>
#include "GPG2DC_Base.h"
#include <base/list/ListDL.h>
#include <base/MM.h>

namespace rpg3D {
class GPG2DC_Entities : public GPG2DC_Base {priv typedef GPG2DC_Base super;pub dCtor(GPG2DC_Entities);
    priv ListDL<IGrid2DContentModule*> gridEntitiesList{LOC};
    
    pub explicit GPG2DC_Entities(
        base::IGameWorld* gw, 
        Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea
    );

    pub void hookGridEntity(IGrid2DContentModule* grid2DContent, Vector3& pos) final;
    prot virtual void onGridEntityHooked(IGrid2DContentModule* grid2DContent, Vector3& pos);

    pub void releaseGridEntity(IGrid2DContentModule* grid2DContent) final;
    prot virtual void onGridEntityReleasePre(IGrid2DContentModule* grid2DContent);

    pub ~GPG2DC_Entities() override;
};
};
