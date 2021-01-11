#pragma once

#include <base/gh.h>
#include "../RenderPhase.h"
#include "IRP3DShadowMap.h"
#include <graphics/shader/pool/ShadersPool.h>

namespace graphics {
    class IShader;
};

namespace graphics {
class RP3DSM_Base : public RenderPhase, public virtual IRP3DShadowMap {priv typedef RenderPhase super;pub dCtor(RP3DSM_Base);
    priv class ShadersPoolLocal : public ShadersPool {priv typedef ShadersPool super; pub dCtor(ShadersPoolLocal);
        pub explicit ShadersPoolLocal() {};

        prot IShader* createNewShader(
            IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) final;

        pub ~ShadersPoolLocal() override {};
    };

    pub explicit RP3DSM_Base(
        int renderOrder,
        base::IScreen* screen);
    prot void create() override;

    prot void dispose() override;
    pub ~RP3DSM_Base() override;
};
};
