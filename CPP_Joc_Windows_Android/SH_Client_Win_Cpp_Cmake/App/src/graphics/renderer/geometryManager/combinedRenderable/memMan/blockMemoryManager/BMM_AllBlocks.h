#pragma once

#include <base/gh.h>
#include "BMM_Base.h"
#include <base/list/ListDL.h>

namespace graphics {
    class AllocatedMemBlock;
    class AnyMemBlock;
};

namespace graphics {
class BMM_AllBlocks : public BMM_Base {priv typedef BMM_Base super;pub dCtor(BMM_AllBlocks);
    pub ListDL<AnyMemBlock> allMemBlocksList{};

    pub explicit BMM_AllBlocks(
        GeometryBufferManager* geometryBufferManager,
        AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
        int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
        IDefragmenterBlockMemory* defragmenter);

    pub ~BMM_AllBlocks() override;
};
};
