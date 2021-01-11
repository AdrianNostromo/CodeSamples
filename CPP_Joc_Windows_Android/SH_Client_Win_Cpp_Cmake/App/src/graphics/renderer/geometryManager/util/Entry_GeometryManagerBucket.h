#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider.h>

class IListEntry;
namespace graphics {
    class IRenderable;
    class GeometryProviderInstance;
    class IGeometryBucket;
    class ICombinedRenderable;
};

// This class is used to connect the providers and to have a convenient things_to_render list.
namespace graphics {
class Entry_GeometryManagerBucket {pub dCtor(Entry_GeometryManagerBucket);
    pub class ProviderInstanceEntry {
        pub GeometryProviderInstance* gpInstances = nullptr;
        pub IListEntry* inGPInstance_listEntry = nullptr;
    };

    pub IGeometryBucket* geometryBucket;
    // Use a full ::Entry because it is used for insertAfter for 2d pre-sorting.
    pub ListDL<Entry_GeometryManagerBucket>::Entry* inGeometryBucket_listEntry = nullptr;

    pub ProviderInstanceEntry gpInstancesArray[GEOMETRY_PROVIDER_TYPES_COUNT]{};

    // This is used for an optimisation to jump to the next cRenderable when the gmEntry is already in the next one.
    // This is also used to update the useCount for the entry_cRend for mTransforms to fast_test if removals are required.
    // This is also used to fast_detect if a entry is already in a different cRend (instead of testing each part of it).
    //asd_01;// implement this logic.
    // The jump to the next cRenderable occurs only if the current cRenderable fillRate is above 75%. If the fill rate is lower, insta move to the current cRenderable.
    /// The low fill rate is used because the defragmenter is susposed to do the movements to keep a fill rate of ~90%.
    pub ICombinedRenderable* usesCombinedRenderable = nullptr;

    // This is used only for vertices and indices because they are configured and appear once for each Entry_GeometryManagerBucket. Multiple occurences can occur in the same cRend.
    // Model transforms entries are in a array in GeometryProviderInstance because they can occur only once for each cRend.
    // To fetch entries from this, call a getter from Entry_GeometryManagerBucket and it handles everithing.
    pub InCombinedRenderableBlock* inCombinedRenderableProviderBlocks[GEOMETRY_PROVIDER_TYPES_COUNT]{nullptr};

    pub explicit Entry_GeometryManagerBucket(IGeometryBucket* geometryBucket);

    pub void remove();

    pub InCombinedRenderableBlock* getInCombinedRenderableBlockIfExists(int providerType, int combinedRenderableUIndex);
    pub InCombinedRenderableBlock* getInCombinedRenderableBlockIfUsed(int providerType, int combinedRenderableUIndex);
    pub InCombinedRenderableBlock* getOrCreateInCombinedRenderableBlock(int providerType, int combinedRenderableUIndex);

    pub virtual ~Entry_GeometryManagerBucket();
};
};
