#pragma once

#include <base/gh.h>

namespace graphics {
    class IEnvironment;
};

class IRenderableRenderer {
    // This is used to update the gl buffers.
    pub virtual void prepareForRendering(graphics::IEnvironment* environment) = 0;
    pub virtual void render(graphics::IEnvironment* environment) = 0;

    pub virtual ~IRenderableRenderer() = default;
};
