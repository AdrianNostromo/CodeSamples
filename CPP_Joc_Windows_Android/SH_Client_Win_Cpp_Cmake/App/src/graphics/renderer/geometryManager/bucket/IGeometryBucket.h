#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>

namespace graphics {
    class Entry_GeometryManagerBucket;
    class IGeometryProvider_Vertices;
    class IGeometryProvider_Indices;
    class IGeometryProvider_ModelTransforms;
    class GeometryProviderInstance;
    class CombinedRenderable;
};

namespace graphics {
class IGeometryBucket {
    pub void* rawSubclassPointer = nullptr;

    pub virtual Entry_GeometryManagerBucket* addRenderable(
        IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
        IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms,
        bool usesInsertAfterMode, Entry_GeometryManagerBucket* insertAfterGManEntry) = 0;
    pub virtual void removeEntry(Entry_GeometryManagerBucket* entry_geometryBucket) = 0;

    pub virtual void invalidate(GeometryProviderInstance* gpInstance, int invalidationBitMask) = 0;
    
    pub virtual void removeGeometryProviderInstance(GeometryProviderInstance* providerInstance) = 0;

    pub virtual int getEntriesCount() = 0;

    pub virtual void tickLogic() = 0;

    pub virtual ListDL<CombinedRenderable>& getCombinedRenderablesList() = 0;

    pub virtual ~IGeometryBucket() = default;
};
};
