#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>

#define GEOMETRY_PROVIDER_TYPES_COUNT 3

namespace graphics {
    class GeometryProviderInstance;
    class IGeometryBucket;
    class InCombinedRenderableBlock;
};

namespace graphics {
class IGeometryProvider {
    pub class InvalidationBitGroups {
        // These are used for each provider type.
        pub static const int countAndData_bitGroup;
        pub static const int dataSameCount_bitGroup;

        // This is used to combine invalidation flags for multiple providers in a single
        pub static const int GROUPS_COUNT;
    };

    prot static int GetNew_PROVIDER_TYPE();

    pub virtual int getProviderType() = 0;
    prot virtual void* getProviderInterfaceVoidPointer() = 0;

    // The entries are managed in the gMan.
    priv ListDL<GeometryProviderInstance*> instancesListUnmanaged;
    pub GeometryProviderInstance* getOrCreateFloatableInstance(IGeometryBucket* geometryBucket, ListDL<GeometryProviderInstance>& entriesListManaged_Vertices);
    pub void disconnectFloatingInstance(GeometryProviderInstance* instance);
    prot void invalidate(int invalidationBitMask);

    pub virtual ~IGeometryProvider();
};

};
