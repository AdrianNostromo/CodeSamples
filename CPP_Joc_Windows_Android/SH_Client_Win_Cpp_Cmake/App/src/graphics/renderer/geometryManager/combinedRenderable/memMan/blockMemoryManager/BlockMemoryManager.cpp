#include "BlockMemoryManager.h"
#include <base/math/Math.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/EmptyMemBlockCountSorted.h>

using namespace graphics;

BlockMemoryManager::BlockMemoryManager(
	GeometryBufferManager* geometryBufferManager,
	AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
	int endInsertionBlock_availableEntriesCount, int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
	IDefragmenterBlockMemory* defragmenter)
	: super(
		geometryBufferManager,
		allocationInsertionType, trackEmptyFragmentsByCountSorting, stride,
		endInsertionBlock_availableEntriesCount, totalEntriesCapacity, usesRightToLeftVolatileMemory,
		defragmenter)
{
	if (trackEmptyFragmentsByCountSorting) {
		fragmentEmptyBlocks_countOrder = new TreeAVL<EmptyMemBlockCountSorted>();
	}
}

BlockMemoryManager::~BlockMemoryManager() {
	//void
}
