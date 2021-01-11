#pragma once

#include <base/gh.h>
#include "R_Base.h"
#include "renderablesManager/util/IHandlerRenderablesManager.h"

namespace graphics {
    class RenderablesManager;
};

namespace graphics {
class R_RenderablesManager : public R_Base, public virtual IHandlerRenderablesManager {priv typedef R_Base super;pub dCtor(R_RenderablesManager);
    priv RenderablesManager* renderablesManager = nullptr;

    pub explicit R_RenderablesManager();
    prot void createMain() override;

    pub void prepareForRendering(graphics::IEnvironment* environment) override;

    pub IRenderablesManager* getRenderablesManager() final;

    prot void disposeMain() override;
    pub ~R_RenderablesManager() override;
};
};
