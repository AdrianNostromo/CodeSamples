#pragma once

#include <base/gh.h>
#include "R_RenderablesManager.h"
#include <base/list/Array1D.h>

namespace graphics {
class R_GeometryManager : public R_RenderablesManager {priv typedef R_RenderablesManager super;pub dCtor(R_GeometryManager);
    priv GeometryManager* geometryManager = nullptr;
    
    pub explicit R_GeometryManager();
    prot void createMain() override;

    pub IGeometryManager* getGeometryManager() final;

    prot void onRenderPhaseAdded(IRenderPhase* renderPhase) override;
    prot void onRenderPhaseRemovePre(IRenderPhase* renderPhase) override;

    pub void render(graphics::IEnvironment* environment) override;

    prot void dispose() override;
    pub ~R_GeometryManager() override;
};
};
