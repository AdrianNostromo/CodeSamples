#include "AllocatedMemBlock.h"
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>

using namespace graphics;

int AllocatedMemBlock::DirtyDataBitGroups::Main = 0 | 1 << 0/*bitIndex*/;
int AllocatedMemBlock::DirtyDataBitGroups::RemapingMTransSlot = 0 | 1 << 1/*bitIndex*/;

AllocatedMemBlock::AllocatedMemBlock(
    int blockEntriesOffsetSideAligned, int blockEntriesCount, GeometryBufferManager* memMan,
    Entry_GeometryManagerBucket* dataSource_bucketEntry, GeometryProviderInstance* dataSource_gpi_forMt, ICRB_ModelTransforms* dataSource_icrb_forRemapingMt)
    : blockEntriesOffsetSideAligned(blockEntriesOffsetSideAligned), blockEntriesCount(blockEntriesCount), memMan(memMan),
    dataSource_bucketEntry_forVI(dataSource_bucketEntry), dataSource_gpi_forMt(dataSource_gpi_forMt), dataSource_icrb_forRemapingMt(dataSource_icrb_forRemapingMt)
{
    //void
}

bool AllocatedMemBlock::operator==(AllocatedMemBlock const& other) noexcept {
    return blockEntriesOffsetSideAligned == other.blockEntriesOffsetSideAligned;
}

bool AllocatedMemBlock::operator!=(AllocatedMemBlock const& other) noexcept {
    return blockEntriesOffsetSideAligned != other.blockEntriesOffsetSideAligned;
}

bool AllocatedMemBlock::operator>(AllocatedMemBlock const& other) noexcept {
    return blockEntriesOffsetSideAligned > other.blockEntriesOffsetSideAligned;
}

bool AllocatedMemBlock::operator<(AllocatedMemBlock const& other) noexcept {
    return blockEntriesOffsetSideAligned < other.blockEntriesOffsetSideAligned;
}

void AllocatedMemBlock::invalidateData(int bitGoups) {
    if (inDirtyDataBlocks_listEntry == nullptr) {
        inDirtyDataBlocks_listEntry = memMan->dirtyDataBlocksList.appendDirect(this);
        
        dirtyDataBitMask = bitGoups;
    } else {
        dirtyDataBitMask |= bitGoups;
    }
}

AllocatedMemBlock::~AllocatedMemBlock() {
	//void
}
