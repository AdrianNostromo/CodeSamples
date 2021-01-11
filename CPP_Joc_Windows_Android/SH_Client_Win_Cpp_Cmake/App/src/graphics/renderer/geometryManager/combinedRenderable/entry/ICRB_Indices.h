#pragma once

#include <base/gh.h>
#include "InCombinedRenderableBlock.h"

namespace graphics {
class ICRB_Indices : public InCombinedRenderableBlock {priv typedef InCombinedRenderableBlock super;pub dCtor(ICRB_Indices);
    pub explicit ICRB_Indices();

    pub static InCombinedRenderableBlock* GetNew();

    pub ~ICRB_Indices() override;
};
};
