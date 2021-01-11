#pragma once

#include <base/gh.h>
#include "RP_Rendering.h"

namespace graphics {
class RenderPhase : public RP_Rendering {priv typedef RP_Rendering super;pub dCtor(RenderPhase);
    pub explicit RenderPhase(
        int renderOrder, bool useDepthTest,
        base::IScreen* screen,
        long supportedAutomaticAttributesBitMask, long ignorredAutomaticAttributesBitMask);

    pub ~RenderPhase() override;
};
};
