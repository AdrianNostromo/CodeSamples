#pragma once

#include <base/gh.h>
#include "GM_Validation.h"

namespace graphics {
class GeometryManager : public GM_Validation {priv typedef GM_Validation super;pub dCtor(GeometryManager);
    pub explicit GeometryManager();

    pub ~GeometryManager() override;
};
};
