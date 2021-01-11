#include "AnyMemBlock.h"
#include <cassert>

using namespace graphics;

AnyMemBlock::AnyMemBlock(IBlockMemoryManager* blockMemoryManager, int blockEntriesOffsetLeftToRight, int blockEntriesCount)
	: blockMemoryManager(blockMemoryManager), blockEntriesOffsetLeftToRight(blockEntriesOffsetLeftToRight), blockEntriesCount(blockEntriesCount)
{
	//void
}

AnyMemBlock::~AnyMemBlock() {
	if (allocatedMemBlock != nullptr || emptyMemBlockOffsetSorted != nullptr) {
		// On dealloc, these should be disconnected.
		assert(false);
	}
}
