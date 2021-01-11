#pragma once

#include <base/gh.h>
#include "BMM_EmptyBlocks.h"
#include <base/list/ListDL.h>

namespace graphics {
    class AllocatedMemBlock;
    class AnyMemBlock;
};

namespace graphics {
class BMM_Allocator : public BMM_EmptyBlocks {priv typedef BMM_EmptyBlocks super;pub dCtor(BMM_Allocator);
    pub explicit BMM_Allocator(
        GeometryBufferManager* geometryBufferManager,
        AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
        int endInsertionBlock_availableEntriesCount, int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
        IDefragmenterBlockMemory* defragmenter);

    pub AllocatedMemBlock* allocate(int blockEntriesCount, Entry_GeometryManagerBucket* dataSource_bucketEntry, GeometryProviderInstance* dataSource_gpi_forMt, ICRB_ModelTransforms* dataSource_icrb_forRemapingMt);
    pub std::nullptr_t deallocate(AllocatedMemBlock* memBlock);

    priv AnyMemBlock* allocateBufferBlock(int blockEntriesCount);
    priv void releaseBufferBlock(AnyMemBlock* memBlockAny);

    priv void syncFillRate();

    pub ~BMM_Allocator() override;
};
};
