#include "BMM_AllBlocks.h"
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AllocatedMemBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AnyMemBlock.h>

using namespace graphics;

BMM_AllBlocks::BMM_AllBlocks(
	GeometryBufferManager* geometryBufferManager,
	AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
	int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
	IDefragmenterBlockMemory* defragmenter)
	: super(
		geometryBufferManager,
		allocationInsertionType, trackEmptyFragmentsByCountSorting, stride,
		totalEntriesCapacity, usesRightToLeftVolatileMemory,
		defragmenter)
{
	//void
}

BMM_AllBlocks::~BMM_AllBlocks() {
	//void
}
