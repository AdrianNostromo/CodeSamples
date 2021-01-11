#pragma once

#include <base/gh.h>
#include "InCombinedRenderableBlock.h"

namespace graphics {
class ICRB_Vertices : public InCombinedRenderableBlock {priv typedef InCombinedRenderableBlock super;pub dCtor(ICRB_Vertices);
    pub explicit ICRB_Vertices();

    pub static InCombinedRenderableBlock* GetNew();

    pub ~ICRB_Vertices() override;
};
};
