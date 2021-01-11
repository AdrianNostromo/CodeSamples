#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>

namespace graphics {
    class Entry_RenderablesManager;
    class IGeometryProvider_Vertices;
    class IGeometryProvider_Indices;
    class IGeometryProvider_ModelTransforms;
};

namespace graphics {
class IRenderable {
    pub virtual Entry_RenderablesManager*& getEntry_renderablesManagerRef() = 0;

    pub virtual IGeometryProvider_Vertices* getGeometryProvider_Vertices() = 0;
    pub virtual IGeometryProvider_Indices* getGeometryProvider_Indices() = 0;
    pub virtual IGeometryProvider_ModelTransforms* getGeometryProvider_ModelTransforms() = 0;

    pub virtual ~IRenderable() = default;
};
};
