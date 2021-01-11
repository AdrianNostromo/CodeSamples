#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IRenderer.h"

namespace graphics {
class R_Base : public base::ManagedObject, virtual public IRenderer {priv typedef base::ManagedObject super;pub dCtor(R_Base);
    pub explicit R_Base();

    pub void prepareForRendering(graphics::IEnvironment* environment) override;
    pub void render(graphics::IEnvironment* environment) override;

    prot virtual void onRenderPhaseAdded(IRenderPhase* renderPhase);
    prot virtual void onRenderPhaseRemovePre(IRenderPhase* renderPhase);

    pub ~R_Base() override;
};
};
