#pragma once

#include <base/gh.h>
#include "CR_Data.h"
#include <base/list/ListDL.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>

namespace graphics {
    class AllocatedMemBlock;
};

namespace graphics {
class CombinedRenderable : public CR_Data {priv typedef CR_Data super;pub dCtor(CombinedRenderable);
    pub class Profiling {
        pub int testVal1 = 1;
    };

    // Use this subclass instead of the interface because the logger will access mm_... and other low level objects.
    pub static ListDL<CombinedRenderable*> globalInstancesList;
    pub IListEntry* inGlobalList_cEntry;

    pub Profiling profiling{};

    pub ListDL<CombinedRenderable>::Entry* inGeometryManager_listEntry = nullptr;

    // This is used for a optimisation to avoid indices writing if not changed.
    /// This is cleared each frame before enumerating the entries.
    /// This saves the last inserted indices mem block and when the next one is processed, checks if it needs to be deallocated and allocated again.
    /// The logic also handles if the desired mem region is occupied by other indices and deallocates them.
    priv AllocatedMemBlock* lastEnumerationProcessedMemBlock_indices = nullptr;
    priv AllocatedMemBlock* lastEnumerationProcessedMemBlock_modelTransforms = nullptr;
    
    pub explicit CombinedRenderable(
        std::shared_ptr<VertexAttributesList> vertexAttributesList, 
        std::shared_ptr<graphics::Material> material,
        int maxVerticesCount, int maxIndicesCount, int maxWorldTransformsCount);
    pub void create() override;

    pub void prepareForEnumerationInsertionPhase();

    pub bool hasCapacityForGeometryBucketEntry(
        Entry_GeometryManagerBucket* entryGBucket, 
        GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices, GeometryProviderInstance* gpi_ModelTransforms);
    pub void smartPushGeometryBucketEntry(
        Entry_GeometryManagerBucket* entryGBucket, 
        GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices, GeometryProviderInstance* gpi_ModelTransforms);
    
    priv void handleDesiredIndicesRegionClearing(AllocatedMemBlock* existingtIndicesMemBlock, int entriesCount);
    // This removes vertices and indice instantly. For mTrans, it saves the entries in a toRemoveList and they are removed at a later logic step (This is because mTrans may be used by multiple entries in the same cRend and removing it in here is bendwidth inefficient).
    // This is called when a bucket entry is removed or it is removed to be inserted in a different cRenderable.
    pub void removeSemiManagedGeometryBucketEntry(
        Entry_GeometryManagerBucket* entryGBucket, 
        GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices, GeometryProviderInstance* gpi_ModelTransforms,
        ListDL<ICRB_ModelTransforms::ToRemoveUnusedEntry>& markedForRemovalGPIModelTransformsList) final;

    // Call this only if the gpInstance is in this cRenderable (check before call).
    //entryGBucket_forVerticesIndices is optional and used for vertices and indices info fetching only.
    //gpInstance_forModelTransforms is optional and used for model_transforms info fetching only.
    pub void removeGeometryProviderInstanceDataMustExist(
        int providerType, 
        Entry_GeometryManagerBucket* entryGBucket_forVerticesIndices, GeometryProviderInstance* gpInstance_forModelTransforms) final;
    pub void releaseTransformsRemapingSlot(ICRB_ModelTransforms* icrb_ModelTransforms) final;

    pub void writeChangedDatas();

    pub int getIndicesCount() final;

    pub void dispose() override;
    pub ~CombinedRenderable() override;
};
};
