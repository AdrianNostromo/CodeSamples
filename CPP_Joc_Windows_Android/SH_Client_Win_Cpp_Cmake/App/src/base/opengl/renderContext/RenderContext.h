#pragma once

#include <base/gh.h>
#include <base/opengl/IGL.h>
#include "RC_Bindings.h"

namespace opengl {
class RenderContext : public RC_Bindings {priv typedef RC_Bindings super;pub dCtor(RenderContext);
    pub explicit RenderContext();

    pub virtual ~RenderContext();
};
};
