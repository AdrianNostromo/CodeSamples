#pragma once

#include <base/gh.h>
#include "IRenderContext.h"

namespace opengl {
class RC_Base : public virtual IRenderContext {pub dCtor(RC_Base);
    pub explicit RC_Base();

    pub virtual void onGlContextCreated();
    pub virtual void onGlContextDestroyedPre();

    pub ~RC_Base() override;
};
};
