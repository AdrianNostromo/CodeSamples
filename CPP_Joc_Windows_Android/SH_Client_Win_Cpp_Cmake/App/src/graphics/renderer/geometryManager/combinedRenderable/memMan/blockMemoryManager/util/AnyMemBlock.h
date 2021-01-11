#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>

namespace graphics {
    class AllocatedMemBlock;
    class EmptyMemBlockOffsetSorted;
    class IBlockMemoryManager;
};

namespace graphics {
class AnyMemBlock {pub dCtor(AnyMemBlock);
    pub IBlockMemoryManager* blockMemoryManager;

    // Offset and count are required here because on creation this object is returned
    pub int blockEntriesOffsetLeftToRight;
    pub int blockEntriesCount;

    // Only one of these are used at a time.
    pub AllocatedMemBlock* allocatedMemBlock = nullptr;
    pub EmptyMemBlockOffsetSorted* emptyMemBlockOffsetSorted = nullptr;

    pub ListDL<AnyMemBlock>::Entry* selfCEntry;

    pub explicit AnyMemBlock(IBlockMemoryManager* blockMemoryManager, int blockEntriesOffsetLeftToRight, int blockEntriesCount);

    pub virtual ~AnyMemBlock();
};
};
