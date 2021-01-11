#include "EmptyMemBlockCountSorted.h"
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/EmptyMemBlockOffsetSorted.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AnyMemBlock.h>

using namespace graphics;

EmptyMemBlockCountSorted::EmptyMemBlockCountSorted(EmptyMemBlockOffsetSorted* mainBlock)
	: mainBlock(mainBlock)
{
	//void
}

bool EmptyMemBlockCountSorted::operator==(EmptyMemBlockCountSorted const& other) const noexcept {
	if (mainBlock->memBlockAny->blockEntriesCount != other.mainBlock->memBlockAny->blockEntriesCount || mainBlock->memBlockAny->blockEntriesOffsetLeftToRight != other.mainBlock->memBlockAny->blockEntriesOffsetLeftToRight) {
		return false;
	}

	return true;
}

bool EmptyMemBlockCountSorted::operator!=(EmptyMemBlockCountSorted const& other) const noexcept {
	if (mainBlock->memBlockAny->blockEntriesCount != other.mainBlock->memBlockAny->blockEntriesCount || mainBlock->memBlockAny->blockEntriesOffsetLeftToRight != other.mainBlock->memBlockAny->blockEntriesOffsetLeftToRight) {
		return true;
	}

	return false;
}

bool EmptyMemBlockCountSorted::operator>(EmptyMemBlockCountSorted const& other) const noexcept {
	if (mainBlock->memBlockAny->blockEntriesCount != other.mainBlock->memBlockAny->blockEntriesCount) {
		return mainBlock->memBlockAny->blockEntriesCount > other.mainBlock->memBlockAny->blockEntriesCount;
	}

	return mainBlock->memBlockAny->blockEntriesOffsetLeftToRight > other.mainBlock->memBlockAny->blockEntriesOffsetLeftToRight;
}

bool EmptyMemBlockCountSorted::operator<(EmptyMemBlockCountSorted const& other) const noexcept {
	if (mainBlock->memBlockAny->blockEntriesCount != other.mainBlock->memBlockAny->blockEntriesCount) {
		return mainBlock->memBlockAny->blockEntriesCount < other.mainBlock->memBlockAny->blockEntriesCount;
	}

	return mainBlock->memBlockAny->blockEntriesOffsetLeftToRight < other.mainBlock->memBlockAny->blockEntriesOffsetLeftToRight;
}

bool EmptyMemBlockCountSorted::operator==(const ComparatorCountKey& other) const noexcept {
	return mainBlock->memBlockAny->blockEntriesCount == other.blockEntriesCount;
}

bool EmptyMemBlockCountSorted::operator!=(const ComparatorCountKey& other) const noexcept {
	return mainBlock->memBlockAny->blockEntriesCount != other.blockEntriesCount;
}

bool EmptyMemBlockCountSorted::operator>(const ComparatorCountKey& other) const noexcept {
	return mainBlock->memBlockAny->blockEntriesCount > other.blockEntriesCount;
}

bool EmptyMemBlockCountSorted::operator<(const ComparatorCountKey& other) const noexcept {
	return mainBlock->memBlockAny->blockEntriesCount < other.blockEntriesCount;
}

bool EmptyMemBlockCountSorted::operator>=(const ComparatorCountKey& other) const noexcept {
	return mainBlock->memBlockAny->blockEntriesCount >= other.blockEntriesCount;
}

bool EmptyMemBlockCountSorted::operator<=(const ComparatorCountKey& other) const noexcept {
	return mainBlock->memBlockAny->blockEntriesCount <= other.blockEntriesCount;
}

EmptyMemBlockCountSorted::~EmptyMemBlockCountSorted() {
	//void
}
