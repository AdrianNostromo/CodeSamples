#pragma once

#include <base/gh.h>
#include "R_RenderPhases.h"

namespace graphics {
class Renderer : public R_RenderPhases {priv typedef R_RenderPhases super;pub dCtor(Renderer);
    pub explicit Renderer();

    pub ~Renderer() override;
};
};
