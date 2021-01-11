#pragma once

#include <base/gh.h>
#include "BMM_Allocator.h"

namespace graphics {
class BlockMemoryManager : public BMM_Allocator {priv typedef BMM_Allocator super;pub dCtor(BlockMemoryManager);
    
    pub explicit BlockMemoryManager(
        GeometryBufferManager* geometryBufferManager,
        AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
        int endInsertionBlock_availableEntriesCount, int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
        IDefragmenterBlockMemory* defragmenter);

    pub virtual ~BlockMemoryManager();
};
};
