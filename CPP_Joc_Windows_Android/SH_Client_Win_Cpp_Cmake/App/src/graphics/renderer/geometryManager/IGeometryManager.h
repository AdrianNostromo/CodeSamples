#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>

namespace graphics {
    class IGeometryProvider_Vertices;
    class IGeometryProvider_Indices;
    class IGeometryProvider_ModelTransforms;
    class Entry_GeometryManagerBucket;
    class GeometryProviderInstance;
    class IRenderable;
};

namespace graphics {
class IGeometryManager {
    pub virtual void addRenderable(
        IRenderable* renderable, ListDL<Entry_GeometryManagerBucket*>& gManagerBucketEntriesList,
        bool usesInsertAfterMode, IRenderable* insertAfterNodePartTarget) = 0;

    pub virtual void tickLogic() = 0;

    pub virtual ~IGeometryManager() = default;
};
};
