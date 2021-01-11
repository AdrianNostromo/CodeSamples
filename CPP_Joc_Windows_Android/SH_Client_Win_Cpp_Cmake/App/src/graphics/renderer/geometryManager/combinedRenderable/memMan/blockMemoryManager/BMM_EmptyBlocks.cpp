#include "BMM_EmptyBlocks.h"
#include <base/exceptions/LogicException.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AllocatedMemBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/EmptyMemBlockOffsetSorted.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/EmptyMemBlockCountSorted.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AnyMemBlock.h>

using namespace graphics;

BMM_EmptyBlocks::ComparatorContactNeighbourOnlyOffsetOrder::ComparatorContactNeighbourOnlyOffsetOrder(int targetBlockEntriesOffset, int targetBlockEntriesCount)
	: targetBlockEntriesOffset(targetBlockEntriesOffset), targetBlockEntriesCount(targetBlockEntriesCount)
{
	//void
}

int BMM_EmptyBlocks::ComparatorContactNeighbourOnlyOffsetOrder::operator()(EmptyMemBlockOffsetSorted& o) const {
	if (targetBlockEntriesOffset + targetBlockEntriesCount < o.memBlockAny->blockEntriesOffsetLeftToRight) {
		// The key is on the left and not in contact with the tree entry.
		return -1;
	} else if (targetBlockEntriesOffset > o.memBlockAny->blockEntriesOffsetLeftToRight + o.memBlockAny->blockEntriesCount) {
		// The key is on the right and not in contact with the tree entry.
		return 1;
	}

	// Key range is inside a tree entry or is in contact with it. Do more checks below
	int boundsMin = targetBlockEntriesOffset >= o.memBlockAny->blockEntriesOffsetLeftToRight ? targetBlockEntriesOffset : o.memBlockAny->blockEntriesOffsetLeftToRight;
	int boundsMax = targetBlockEntriesOffset + targetBlockEntriesCount <= o.memBlockAny->blockEntriesOffsetLeftToRight + o.memBlockAny->blockEntriesCount ? targetBlockEntriesOffset + targetBlockEntriesCount : o.memBlockAny->blockEntriesOffsetLeftToRight + o.memBlockAny->blockEntriesCount;
	if (boundsMax > boundsMin) {
		// The region overlapps with the key, a bug exists.
		throw LogicException(LOC);
	}

	// The key is in neighbour_only contact. There is no overlapp.
	return 0;
}

BMM_EmptyBlocks::BMM_EmptyBlocks(
	GeometryBufferManager* geometryBufferManager,
	AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
	int endInsertionBlock_availableEntriesCount, int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
	IDefragmenterBlockMemory* defragmenter)
	: super(
		geometryBufferManager,
		allocationInsertionType, trackEmptyFragmentsByCountSorting, stride,
		totalEntriesCapacity, usesRightToLeftVolatileMemory,
		defragmenter),
	endInsertionBlock_availableEntriesCount(endInsertionBlock_availableEntriesCount)
{
	//void
}

int BMM_EmptyBlocks::getIndicesCount() {
	return endInsertionBlock_entriesOffset;
}

int BMM_EmptyBlocks::getInsertEntrisCapacityWithAllocationInsertionType() {
	if (allocationInsertionType == AllocationInsertionType::EndInsertionBlockOnly) {
		return endInsertionBlock_availableEntriesCount;
	} else if (allocationInsertionType == AllocationInsertionType::FirstRegionMustHaveCapacity || allocationInsertionType == AllocationInsertionType::FirstAvailableSizeCheckedRegion) {
		return endInsertionBlock_availableEntriesCount + totalFragmentedEntriesCount;
	} else {
		throw LogicException(LOC);
	}
}

BMM_EmptyBlocks::~BMM_EmptyBlocks() {
	if (fragmentEmptyBlocks_countOrder != nullptr) {
		delete fragmentEmptyBlocks_countOrder;
		fragmentEmptyBlocks_countOrder = nullptr;
	}
}
