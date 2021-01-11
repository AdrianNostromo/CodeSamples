#pragma once

#include <base/gh.h>
#include <base/container/util/ICustomComparator1.h>
#include <base/list/ListDL.h>
#include <base/container/TreeAVL.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/BlockMemoryManager.h>

namespace graphics {
    class ICombinedRenderable;
    class BufferObject;
    class VertexBufferObject;
    class IndexBufferObject;
    class UniformBufferObject;
    class AllocatedMemBlock;
    class Entry_GeometryManagerBucket;
    class InCombinedRenderableBlock;
    class GeometryProviderInstance;
    class ICRB_ModelTransforms;
    class IDefragmenterBlockMemory;
};

namespace graphics {
class GeometryBufferManager {pub dCtor(GeometryBufferManager);
    pub ICombinedRenderable* combinedRenderable;

    // Use a common dirty blocks list for all blockMM_ because it isolated the update code of CombinedRenderable from the low level mem man code of BlockMemoryManager.
    pub ListDL<AllocatedMemBlock*> dirtyDataBlocksList{};
    // Allocations occur only from volatile memory.
    pub BlockMemoryManager* blockMM_stable = nullptr;
    pub BlockMemoryManager* blockMM_volatile = nullptr;

    pub BufferObject* bufferObject = nullptr;
    // Use void* to avoid array access errors.
    pub void* data = nullptr;
    pub int stride = -1;
    // Used by model_trans to get the vertex attribute offset for a_modelTransIndex.
    pub int vaByteOffset_worldTransformIndex;

    // Used by model_trans for the normal buffer.
    // These are optional and are tracked to the model transforms.
    pub BufferObject* bufferObjectB = nullptr;
    pub void* dataB = nullptr;
    pub int strideB = -1;

    // Use by mm_remaping_mTrans to have the number of entries that are packed in each gl var.
    pub int entriesPerGLEntry;

    pub int fillRateCount = 0;
    pub float fillRatePercent = 0.0f;

    pub explicit GeometryBufferManager(
        ICombinedRenderable* combinedRenderable,
        BufferObject* bufferObject, BlockMemoryManager::AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting,
        int vaByteOffset_worldTransformIndex, BufferObject* bufferObjectB,
        int entriesPerGLEntry,
        bool usesStableMemory, bool usesRightToLeftVolatileMemory,
        IDefragmenterBlockMemory* defragmenter_volatile, IDefragmenterBlockMemory* defragmenter_stable);

    pub int getIndicesCount();

    pub VertexBufferObject* getBufferObject_asVBO();
    pub IndexBufferObject* getBufferObject_asIBO();
    pub UniformBufferObject* getBufferObject_asUBO();
    pub UniformBufferObject* getBufferObjectB_asUBO();

    // This takes allocationInsertionType into consideration.
    pub int getInsertEntrisCapacityWithAllocationInsertionType();

    pub int getTotalEntriesCapacity();
    pub int getTotalFragmentedEntriesCount();

    pub void onBlockMMEndInsertionBlockOffsetChanged();

    // Allocations occur only from volatile memory.
    pub AllocatedMemBlock* allocate(int blockEntriesCount, Entry_GeometryManagerBucket* dataSource_bucketEntry, GeometryProviderInstance* dataSource_gpi_forMt, ICRB_ModelTransforms* dataSource_icrb_forRemapingMt);
    pub std::nullptr_t deallocate(AllocatedMemBlock* memBlock);

    pub void tickDefrag();

    pub virtual ~GeometryBufferManager();
};
};
