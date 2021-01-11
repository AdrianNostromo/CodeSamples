
#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>

namespace graphics {
    class IGeometryManager;
};

namespace graphics {
class IHandlerRenderablesManager {
    pub virtual IGeometryManager* getGeometryManager() = 0;

    pub virtual ~IHandlerRenderablesManager() = default;
};
};
