#pragma once

#include <base/gh.h>
#include "BMM_AllBlocks.h"
#include <base/container/TreeAVL.h>

namespace graphics {
    class EmptyMemBlockOffsetSorted;
    class EmptyMemBlockCountSorted;
};

namespace graphics {
class BMM_EmptyBlocks : public BMM_AllBlocks {priv typedef BMM_AllBlocks super;pub dCtor(BMM_EmptyBlocks);
    prot struct ComparatorContactNeighbourOnlyOffsetOrder : public ICustomComparator1<EmptyMemBlockOffsetSorted&/*ArgumentType*/, int/*ResultType*/> {
        pub int targetBlockEntriesOffset = -1;
        pub int targetBlockEntriesCount = -1;

        pub explicit ComparatorContactNeighbourOnlyOffsetOrder(int targetBlockEntriesOffset=-1, int targetBlockEntriesCount=-1);

        pub int operator()(EmptyMemBlockOffsetSorted& o) const;
    };

    // Note. This uses a 2 step key (count first and offset after).
    pub TreeAVL<EmptyMemBlockOffsetSorted> fragmentEmptyBlocks_offsetOrder{};
    // This is not used by indices but is used by mTrans. The others may still use this for defragmentations to find the left most region with enough size.
    pub TreeAVL<EmptyMemBlockCountSorted>* fragmentEmptyBlocks_countOrder = nullptr;
    // This is used to know the fragmetation ratio that is used by the defragmented to have a priority.
    pub int totalFragmentedEntriesCount = 0;

    pub int endInsertionBlock_entriesOffset = 0;
    pub int endInsertionBlock_availableEntriesCount;
    // This must contains the size of the entire buffer (_stable and _volatile included) because the below usesRightToLeftVolatileMemory requires that as a offset.
    /// Use a extra variable to compute the actual insert capacity without the other region.

    pub int fillRateCount = 0;

    pub explicit BMM_EmptyBlocks(
        GeometryBufferManager* geometryBufferManager,
        AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
        int endInsertionBlock_availableEntriesCount, int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
        IDefragmenterBlockMemory* defragmenter);

    pub int getIndicesCount();

    // This takes allocationInsertionType into consideration.
    pub int getInsertEntrisCapacityWithAllocationInsertionType();

    pub ~BMM_EmptyBlocks() override;
};
};
