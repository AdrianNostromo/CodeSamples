#pragma once

#include <base/gh.h>
#include "GB_Base.h"
#include <base/list/ListDL.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider.h>

namespace graphics {
    class GeometryBucketUser;
    class IVerticesProvider;
    class ITransformsProvider;
    class IIndicesProvider;
    class Entry_GeometryManagerBucket;
    class GeometryProviderInstance;
    class IGeometryProvider_Vertices;
    class IGeometryProvider_Indices;
    class IGeometryProvider_ModelTransforms;
};

namespace graphics {
class GB_Entries : public GB_Base {priv typedef GB_Base super;pub dCtor(GB_Entries);
    pub ListDL<Entry_GeometryManagerBucket> entriesList_connectedGroups{};

    prot ListDL<GeometryProviderInstance>* entriesList_Vertices = nullptr;
    prot ListDL<GeometryProviderInstance>* entriesList_Indices = nullptr;

    prot ListDL<GeometryProviderInstance>* entriesList_ModelTransforms = nullptr;
    prot ListDL<GeometryProviderInstance>* entriesList_ModelNormalTransforms = nullptr;

    // This is used to remove vertices and indices from combined_renderable.
    prot ListDL<Entry_GeometryManagerBucket*> toRemove_entriesList{};
    // This is used to remove model_transforms from combined_renderable.
    // Only the model_transofrms provider instances are required in this list but all provider types are saved in this list for consistent deletes.
    prot ListDL<GeometryProviderInstance*> toRemove_GPInstancesList[GEOMETRY_PROVIDER_TYPES_COUNT]{
        ListDL<GeometryProviderInstance*>(),
        ListDL<GeometryProviderInstance*>(),
        ListDL<GeometryProviderInstance*>()
    };

    pub explicit GB_Entries(sp<GeometryBucketConfig> config);

    pub int getEntriesCount() final;

    pub Entry_GeometryManagerBucket* addRenderable(
        IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
        IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms,
        bool usesInsertAfterMode, Entry_GeometryManagerBucket* insertAfterGManEntry) final;
    pub void removeEntry(Entry_GeometryManagerBucket* entry_geometryBucket) final;

    void removeGeometryProviderInstance(GeometryProviderInstance* providerInstance) final;

    prot void dispose() override;
    pub ~GB_Entries() override;
};
};
