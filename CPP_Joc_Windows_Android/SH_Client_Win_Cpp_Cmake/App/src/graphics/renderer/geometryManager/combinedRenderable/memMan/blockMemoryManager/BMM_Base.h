#pragma once

#include <base/gh.h>
#include "IBlockMemoryManager.h"

namespace graphics {
    class ICombinedRenderable;
    class BufferObject;
    class VertexBufferObject;
    class IndexBufferObject;
    class UniformBufferObject;
    class Entry_GeometryManagerBucket;
    class InCombinedRenderableBlock;
    class GeometryProviderInstance;
    class ICRB_ModelTransforms;
    class BlockMemoryManager;
    class GeometryBufferManager;
    class IDefragmenterBlockMemory;
};

namespace graphics {
class BMM_Base : public virtual IBlockMemoryManager {pub dCtor(BMM_Base);
    pub enum AllocationInsertionType { EndInsertionBlockOnly, FirstRegionMustHaveCapacity, FirstAvailableSizeCheckedRegion };

    pub GeometryBufferManager* geometryBufferManager;

    prot AllocationInsertionType allocationInsertionType;
    // This is used by the allocation method or by the defrag of volatile memory.
    priv bool trackEmptyFragmentsByCountSorting;

    pub int stride = -1;

    pub int totalEntriesCapacity;

    // This causes the offset of returned mem blocks to be offseted so the allocations actually occur from the buffer right side to the left.
    // Internally left_to_right is always used for logic.
    pub bool usesRightToLeftVolatileMemory;

    pub IDefragmenterBlockMemory* defragmenter = nullptr;

    pub explicit BMM_Base(
        GeometryBufferManager* geometryBufferManager,
        AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
        int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
        IDefragmenterBlockMemory* defragmenter);

    pub ~BMM_Base() override;
};
};
