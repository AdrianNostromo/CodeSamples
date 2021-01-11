#include "EmptyMemBlockOffsetSorted.h"
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AnyMemBlock.h>
#include <cassert>

using namespace graphics;

EmptyMemBlockOffsetSorted::EmptyMemBlockOffsetSorted(AnyMemBlock* memBlockAny)
	: memBlockAny(memBlockAny)
{
	//void
}

dOpsDef(EmptyMemBlockOffsetSorted, memBlockAny->blockEntriesOffsetLeftToRight);

EmptyMemBlockOffsetSorted::~EmptyMemBlockOffsetSorted() {
	if (memBlockAny != nullptr) {
		// On dealloc, this should be disconnected.
		assert(false);
	}
}
