#pragma once

#include <base/gh.h>
#include <base/container/TreeAVL.h>

class IListEntry;
class IContainerNode;
namespace graphics {
    class GeometryBufferManager;
    class Entry_GeometryManagerBucket;
    class GeometryProviderInstance;
    class ICRB_ModelTransforms;
    class AnyMemBlock;
};

namespace graphics {
class AllocatedMemBlock {pub dCtor(AllocatedMemBlock);
    pub class DirtyDataBitGroups {
        pub static int Main;
        // For vertices update, this will update the vertices a_modelTransIndex vertex attribute.
        pub static int RemapingMTransSlot;
    };

    pub int blockEntriesOffsetSideAligned;
    pub int blockEntriesCount;

    pub GeometryBufferManager* memMan;

    pub Entry_GeometryManagerBucket* dataSource_bucketEntry_forVI;
    pub GeometryProviderInstance* dataSource_gpi_forMt;
    pub ICRB_ModelTransforms* dataSource_icrb_forRemapingMt;

    pub AnyMemBlock* memBlockAny = nullptr;

    // Use the actual type because the next node is required.
    pub IListEntry* inDirtyDataBlocks_listEntry = nullptr;
    pub int dirtyDataBitMask = 0;

    pub explicit AllocatedMemBlock(
        int blockEntriesOffsetSideAligned, int blockEntriesCount, GeometryBufferManager* memMan,
        Entry_GeometryManagerBucket* dataSource_bucketEntry, GeometryProviderInstance* dataSource_gpi_forMt, ICRB_ModelTransforms* dataSource_icrb_forRemapingMt);

    pub bool operator==(AllocatedMemBlock const& other) noexcept;
    pub bool operator!=(AllocatedMemBlock const& other) noexcept;
    pub bool operator>(AllocatedMemBlock const& other) noexcept;
    pub bool operator<(AllocatedMemBlock const& other) noexcept;

    pub void invalidateData(int bitGoups);

    pub virtual ~AllocatedMemBlock();
};
};
