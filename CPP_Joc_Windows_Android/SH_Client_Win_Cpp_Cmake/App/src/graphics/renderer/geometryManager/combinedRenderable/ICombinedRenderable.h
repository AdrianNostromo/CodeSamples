#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>

namespace graphics {
    class GeometryProviderInstance;
    class Entry_GeometryManagerBucket;
};

namespace graphics {
class ICombinedRenderable {
    // This is used for the indexed array in GeometryProviderInstance entries.
    // Place this here so it can be accessed without a function call.
    pub int uIndex = -1;

    pub virtual void removeGeometryProviderInstanceDataMustExist(
        int providerType, 
        Entry_GeometryManagerBucket* entryGBucket, GeometryProviderInstance* gpInstance) = 0;
    pub virtual void releaseTransformsRemapingSlot(ICRB_ModelTransforms* icrb_ModelTransforms) = 0;
    pub virtual void removeSemiManagedGeometryBucketEntry(
        Entry_GeometryManagerBucket* entryGBucket,
        GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices, GeometryProviderInstance* gpi_ModelTransforms,
        ListDL<ICRB_ModelTransforms::ToRemoveUnusedEntry>& markedForRemovalGPIModelTransformsList) = 0;

    pub virtual void onMemManFillRateCHanged() = 0;

    pub virtual ~ICombinedRenderable() = default;
};
};
