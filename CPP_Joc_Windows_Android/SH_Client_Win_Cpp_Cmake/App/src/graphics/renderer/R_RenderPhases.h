#pragma once

#include <base/gh.h>
#include "R_GeometryManager.h"
#include <base/list/ListDL.h>
#include <base/map/Map1D.h>
#include <base/MM.h>

namespace graphics {
class R_RenderPhases : public R_GeometryManager {priv typedef R_GeometryManager super;pub dCtor(R_RenderPhases);
    prot ListDL<IRenderPhase*> renderPhasesList{LOC};
    priv Map1D<int, IRenderPhase*> renderPhasesMap{};

    pub explicit R_RenderPhases();

    pub void addRenderPhase(IRenderPhase* renderPhase) final;
    pub void removeRenderPhaseMustExist(int type) final;
    priv void removeRenderPhaseMustExist(IRenderPhase* renderPhase) final;
    pub IRenderPhase* getRenderPhase(int rPhaseType, bool mustExist) final;

    prot void onRenderPhaseAdded(IRenderPhase* renderPhase) override;
    prot void onRenderPhaseRemovePre(IRenderPhase* renderPhase) override;

    pub void prepareForRendering(graphics::IEnvironment* environment) override;
    pub void render(graphics::IEnvironment* environment) override;

    prot void dispose() override;
    pub ~R_RenderPhases() override;
};
};
