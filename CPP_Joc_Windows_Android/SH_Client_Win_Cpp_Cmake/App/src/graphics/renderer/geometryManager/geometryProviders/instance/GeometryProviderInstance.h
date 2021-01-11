#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>
#include <base/list/ArrayList.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/InCombinedRenderableBlock.h>

class IListEntry;
namespace graphics {
    class IGeometryBucket;
    class IGeometryProvider;
    class Entry_GeometryManagerBucket;
    class ICombinedRenderable;
};

namespace graphics {
class GeometryProviderInstance {
    pub IGeometryBucket* geometryBucket;
    pub IListEntry* inGeometryBucket_listEntry = nullptr;

    // This is set to nullptr when the instance becomes floating (marked for removal).
    pub IGeometryProvider* provider;
    // This is required because the above "provider" is set tp nullptr on remove from geometryManager.
    pub int providerType;
    pub void* customProviderRawPtr;
    pub IListEntry* inProvider_listEntry = nullptr;

    pub ListDL<Entry_GeometryManagerBucket*> connectedGMGroupsList{};

    //asd_01;// save this in another place (GM_Validation.invalidGPInstancesListsArray in the new wrappers logic, save a invalidationBitMask for each provider type, use a int[3]...; );
    pub int invalidationBitMask = 0;
    //asd_01;// no longer saving this class in the invalidation list, save the Entry_GeometryManagerBucket.
    pub IListEntry* inGMan_invalidation_listEntry = nullptr;

    // These are made nullptr after the GMan.logicTick if no in the combined_renderable anymore.
    // Managed list.
    // Even if entries exist, check if the gMan entry is set.
    // This is used only for model_transforms because the mTrans appear only maximum once per cRend.
    /// Vertices and indices save this entry in Entry_GeometryManagerBucket because vertices and indices are customised and reused multiple times in the same cRend (once for each Entry_GeometryManagerBucket).
    // To fetch entries from this, call a getter from Entry_GeometryManagerBucket and it handles everithing.
    pub ArrayList<InCombinedRenderableBlock*>* inCombinedRenderableBlockIndexedArray = nullptr;

    pub explicit GeometryProviderInstance(IGeometryBucket* geometryBucket, IGeometryProvider* provider, void* customProviderRawPtr);

    pub int getProviderType();

    pub void connectToGroup(Entry_GeometryManagerBucket* providersGroup);
    pub void disconnectFromGroup(Entry_GeometryManagerBucket* providersGroup);
    
    pub void invalidate(int invalidationBitMask);

    pub virtual ~GeometryProviderInstance();
};
};
