#include "BMM_Allocator.h"
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AllocatedMemBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AnyMemBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/EmptyMemBlockOffsetSorted.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/EmptyMemBlockCountSorted.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>

using namespace graphics;

BMM_Allocator::BMM_Allocator(
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
	//void
}

AllocatedMemBlock* BMM_Allocator::allocate(int blockEntriesCount, Entry_GeometryManagerBucket* dataSource_bucketEntry, GeometryProviderInstance* dataSource_gpi_forMt, ICRB_ModelTransforms* dataSource_icrb_forRemapingMt) {
	if (blockEntriesCount <= 0) {
		// Empty blocks not allowed because empty blocks are innefficient and it will have the same offset as another block and that isn't allowed for binTrees.
		// The caller should use special logic for empty blocks.
		throw LogicException(LOC);
	}

	AnyMemBlock* memBlockAny = allocateBufferBlock(blockEntriesCount);

	int blockEntriesOffsetSideAligned;
	if (usesRightToLeftVolatileMemory) {
		blockEntriesOffsetSideAligned = totalEntriesCapacity - memBlockAny->blockEntriesOffsetLeftToRight - memBlockAny->blockEntriesCount;
	} else {
		blockEntriesOffsetSideAligned = memBlockAny->blockEntriesOffsetLeftToRight;
	}
	memBlockAny->allocatedMemBlock = new AllocatedMemBlock(
		blockEntriesOffsetSideAligned, blockEntriesCount, geometryBufferManager/*memMan*/,
		dataSource_bucketEntry, dataSource_gpi_forMt, dataSource_icrb_forRemapingMt
	);
	memBlockAny->allocatedMemBlock->memBlockAny = memBlockAny;

	return memBlockAny->allocatedMemBlock;
}

std::nullptr_t BMM_Allocator::deallocate(AllocatedMemBlock* memBlock) {
	if (memBlock->memMan == nullptr || memBlock->memBlockAny == nullptr) {
		throw LogicException(LOC);
	}

	if (memBlock->inDirtyDataBlocks_listEntry != nullptr) {
		memBlock->dirtyDataBitMask = 0;

		memBlock->inDirtyDataBlocks_listEntry->remove();
		memBlock->inDirtyDataBlocks_listEntry = nullptr;
	}

	AnyMemBlock* memBlockAny = memBlock->memBlockAny;
	{
		// Delete the entry;
		memBlock->memMan = nullptr;
		memBlock->memBlockAny = nullptr;
	
		delete memBlockAny->allocatedMemBlock;
		memBlockAny->allocatedMemBlock = nullptr;
	}

	releaseBufferBlock(memBlockAny);
	
	return nullptr;
}

AnyMemBlock* BMM_Allocator::allocateBufferBlock(int blockEntriesCount) {
	if (blockEntriesCount <= 0) {
		// Empty blocks not allowed because empty blocks are innefficient and it will have the same offset as another block and that isn't allowed for binTrees.
		// The caller should use special logic for empty blocks.
		throw LogicException(LOC);
	}

	AnyMemBlock* memBlockAny = nullptr;
	bool doEndInsertionBlockAllocation = false;

	if (allocationInsertionType == AllocationInsertionType::FirstRegionMustHaveCapacity || allocationInsertionType == AllocationInsertionType::FirstAvailableSizeCheckedRegion) {
		TreeAVL<EmptyMemBlockOffsetSorted>::Node* nodeByOffset = nullptr;

		if (allocationInsertionType == AllocationInsertionType::FirstRegionMustHaveCapacity) {
			if (fragmentEmptyBlocks_offsetOrder.count() >= 1) {
				nodeByOffset = fragmentEmptyBlocks_offsetOrder.getMinValueNode();
				if (nodeByOffset == nullptr) {
					throw LogicException(LOC);
				}
			}
		} else if (allocationInsertionType == AllocationInsertionType::FirstAvailableSizeCheckedRegion) {
			if (fragmentEmptyBlocks_countOrder == nullptr) {
				// Tree is required for this allocation type, there is a constructor param for this;
				throw LogicException(LOC);
			}

			EmptyMemBlockCountSorted::ComparatorCountKey comparatorCountKey{ blockEntriesCount };
			TreeAVL<EmptyMemBlockCountSorted>::Node* nodeByCount = fragmentEmptyBlocks_countOrder->getNodeEqualOrNextBigger(comparatorCountKey);
			if (nodeByCount != nullptr) {
				nodeByOffset = nodeByCount->data.mainBlock->nodeByOffset;
			}
		} else {
			throw LogicException(LOC);
		}

		if (nodeByOffset == nullptr) {
			doEndInsertionBlockAllocation = true;
		} else {
			EmptyMemBlockOffsetSorted* emptyMemBlockOffsetSorted = &nodeByOffset->data;

			if (emptyMemBlockOffsetSorted->memBlockAny->blockEntriesCount < blockEntriesCount) {
				// This should never occur. Only remaping_mTransforms use this (it has a blockEntriesCount of 1) and indices (it is guaranteed to have a block with enough space from special code logic that removes entries until enough space is guaranteed).
				throw LogicException(LOC);
			}

			totalFragmentedEntriesCount -= blockEntriesCount;

			if (emptyMemBlockOffsetSorted->memBlockAny->blockEntriesCount == blockEntriesCount) {
				// Entire block used, remove it.

				{
					// Keep the existing AnyMemBlock from nodeByOffset and disconnect it from nodeByOffset and return that.
					if (emptyMemBlockOffsetSorted->memBlockAny == nullptr) {
						throw LogicException(LOC);
					}

					memBlockAny = emptyMemBlockOffsetSorted->memBlockAny;

					emptyMemBlockOffsetSorted->memBlockAny->emptyMemBlockOffsetSorted = nullptr;
					emptyMemBlockOffsetSorted->memBlockAny = nullptr;
				}

				if (emptyMemBlockOffsetSorted->nodeByCount != nullptr) {
					emptyMemBlockOffsetSorted->nodeByCount->remove();
					emptyMemBlockOffsetSorted->nodeByCount = nullptr;
				}

				nodeByOffset->remove();
			} else {
				// Only a part of the fragment block is used, shrink it.

				// Create a AnyMemBlock and prepend it before the entry from nodeByOffset;
				ListDL<AnyMemBlock>::Entry* memBlockAnyEntry = allMemBlocksList.insertBeforeEmplace(
					emptyMemBlockOffsetSorted->memBlockAny->selfCEntry, 
					this/*blockMemoryManager*/,
					emptyMemBlockOffsetSorted->memBlockAny->blockEntriesOffsetLeftToRight, blockEntriesCount
				);
				memBlockAnyEntry->data.selfCEntry = memBlockAnyEntry;
				memBlockAny = &memBlockAnyEntry->data;

				emptyMemBlockOffsetSorted->memBlockAny->blockEntriesOffsetLeftToRight += blockEntriesCount;
				emptyMemBlockOffsetSorted->memBlockAny->blockEntriesCount -= blockEntriesCount;

				if (fragmentEmptyBlocks_countOrder != nullptr) {
					if (emptyMemBlockOffsetSorted->nodeByCount == nullptr) {
						throw LogicException(LOC);
					}
					// 2_step sorting (by count first) is used and the node is no longer at the sorted location, need to re-insert it.
					fragmentEmptyBlocks_countOrder->reinsertNode(emptyMemBlockOffsetSorted->nodeByCount);
				}
			}
		}
	} else if (allocationInsertionType == AllocationInsertionType::EndInsertionBlockOnly) {
		doEndInsertionBlockAllocation = true;
	} else {
		throw LogicException(LOC);
	}

	if (doEndInsertionBlockAllocation) {
		// Do end block allocation.
		if (blockEntriesCount > endInsertionBlock_availableEntriesCount) {
			throw LogicException(LOC);
		}

		// Create a AnyMemBlock at the end of the list.
		ListDL<AnyMemBlock>::Entry* memBlockAnyEntry = allMemBlocksList.appendEmplace(
			this/*blockMemoryManager*/,
			endInsertionBlock_entriesOffset, blockEntriesCount
		);
		memBlockAnyEntry->data.selfCEntry = memBlockAnyEntry;
		memBlockAny = &memBlockAnyEntry->data;

		endInsertionBlock_entriesOffset += blockEntriesCount;
		// endInsertionBlock_availableEntriesCount is updated by the following function and keeps count if 2 memory zones are used or not.
		geometryBufferManager->onBlockMMEndInsertionBlockOffsetChanged();
	}

	if (memBlockAny == nullptr) {
		throw LogicException(LOC);
	}

	syncFillRate();

	return memBlockAny;
}

void BMM_Allocator::releaseBufferBlock(AnyMemBlock* memBlockAny) {
	if (memBlockAny == nullptr) {
		throw LogicException(LOC);
	}

	if (memBlockAny->blockEntriesOffsetLeftToRight + memBlockAny->blockEntriesCount == endInsertionBlock_entriesOffset) {
		// Block can be added to the end append block.
		endInsertionBlock_entriesOffset -= memBlockAny->blockEntriesCount;
		// endInsertionBlock_availableEntriesCount is updated by the following function and keeps count if 2 memory zones are used or not.
		geometryBufferManager->onBlockMMEndInsertionBlockOffsetChanged();

		// Remove the entry.
		memBlockAny->selfCEntry->remove();

		// Check the left mem block and if it is a empty fragment, add it also to the end insertion block.
		ListDL<AnyMemBlock>::Entry* lastMemBlockAnyEntry = allMemBlocksList.getLast();
		if (lastMemBlockAnyEntry != nullptr && lastMemBlockAnyEntry->data.emptyMemBlockOffsetSorted != nullptr) {
			// The new last mem block is a empty fragment, attach it to the end insertion block also.
			AnyMemBlock* lastMemBlockAny = &lastMemBlockAnyEntry->data;

			// The tree node elements are no longer fragments.
			totalFragmentedEntriesCount -= lastMemBlockAny->blockEntriesCount;

			endInsertionBlock_entriesOffset -= lastMemBlockAny->blockEntriesCount;
			// endInsertionBlock_availableEntriesCount is updated by the following function and keeps count if 2 memory zones are used or not.
			geometryBufferManager->onBlockMMEndInsertionBlockOffsetChanged();

			{
				if (fragmentEmptyBlocks_countOrder != nullptr) {
					if (lastMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount == nullptr) {
						throw LogicException(LOC);
					}
					lastMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount->remove();
					lastMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount = nullptr;
				}
				lastMemBlockAny->emptyMemBlockOffsetSorted->nodeByOffset->data.memBlockAny = nullptr;
				lastMemBlockAny->emptyMemBlockOffsetSorted->nodeByOffset->remove();
				lastMemBlockAny->emptyMemBlockOffsetSorted = nullptr;

				lastMemBlockAny->selfCEntry->remove();
			}
		}
	} else {
		// Add a empty fragmented entry to the tree (combine with an existing entry if possible, check for a attach_node).
		// The insert may make it possible to connect 2 already existing entries.

		totalFragmentedEntriesCount += memBlockAny->blockEntriesCount;

		bool isCombinedInNeightbour = false;

		ListDL<AnyMemBlock>::Entry* leftMemBlockCEntry = memBlockAny->selfCEntry->prev;
		if (leftMemBlockCEntry != nullptr && leftMemBlockCEntry->data.emptyMemBlockOffsetSorted != nullptr) {
			// Combine into the left neighbour.
			AnyMemBlock* leftMemBlockAny = &leftMemBlockCEntry->data;

			if (leftMemBlockAny->blockEntriesOffsetLeftToRight + leftMemBlockAny->blockEntriesCount != memBlockAny->blockEntriesOffsetLeftToRight) {
				// There should never be overlapp and also blocks must be connected.
				throw LogicException(LOC);
			}

			isCombinedInNeightbour = true;

			leftMemBlockAny->blockEntriesCount += memBlockAny->blockEntriesCount;

			if (fragmentEmptyBlocks_countOrder != nullptr) {
				if (leftMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount == nullptr) {
					throw LogicException(LOC);
				}
				// 2_step sorting (by count first) is used and the node is no longer at the sorted location, need to re-insert it.
				fragmentEmptyBlocks_countOrder->reinsertNode(leftMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount);
			}
		}

		ListDL<AnyMemBlock>::Entry* rightMemBlockCEntry = memBlockAny->selfCEntry->next;
		if (rightMemBlockCEntry != nullptr && rightMemBlockCEntry->data.emptyMemBlockOffsetSorted != nullptr) {
			AnyMemBlock* rightMemBlockAny = &rightMemBlockCEntry->data;

			if (!isCombinedInNeightbour) {
				// Combine into the right neighbour.

				if (memBlockAny->blockEntriesOffsetLeftToRight + memBlockAny->blockEntriesCount != rightMemBlockAny->blockEntriesOffsetLeftToRight) {
					// There should never be overlapp and also blocks must be connected.
					throw LogicException(LOC);
				}

				isCombinedInNeightbour = true;

				rightMemBlockAny->blockEntriesOffsetLeftToRight -= memBlockAny->blockEntriesCount;
				rightMemBlockAny->blockEntriesCount += memBlockAny->blockEntriesCount;

				if (fragmentEmptyBlocks_countOrder != nullptr) {
					if (rightMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount == nullptr) {
						throw LogicException(LOC);
					}
					// 2_step sorting (by count first) is used and the node is no longer at the sorted location, need to re-insert it.
					fragmentEmptyBlocks_countOrder->reinsertNode(rightMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount);
				}
			} else {
				// Already combined with the left neighbour.
				// Combine this right empty fragment into the left one and remove this right neighbour.
				AnyMemBlock* leftMemBlockAny = &leftMemBlockCEntry->data;

				if (leftMemBlockAny->blockEntriesOffsetLeftToRight + leftMemBlockAny->blockEntriesCount != rightMemBlockAny->blockEntriesOffsetLeftToRight) {
					// There should never be overlapp and also blocks must be connected.
					throw LogicException(LOC);
				}

				int entriesCount = rightMemBlockAny->blockEntriesCount;
				
				{
					// Remove right neighbour from lists.
					if (fragmentEmptyBlocks_countOrder != nullptr) {
						if (rightMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount == nullptr) {
							throw LogicException(LOC);
						}
						rightMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount->remove();
						rightMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount = nullptr;
					}
					rightMemBlockAny->emptyMemBlockOffsetSorted->nodeByOffset->data.memBlockAny = nullptr;
					rightMemBlockAny->emptyMemBlockOffsetSorted->nodeByOffset->remove();
					rightMemBlockAny->emptyMemBlockOffsetSorted = nullptr;

					rightMemBlockAny->selfCEntry->remove();
				}

				leftMemBlockAny->blockEntriesCount += entriesCount;
				if (fragmentEmptyBlocks_countOrder != nullptr) {
					if (leftMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount == nullptr) {
						throw LogicException(LOC);
					}
					// 2_step sorting (by count first) is used and the node is no longer at the sorted location, need to re-insert it.
					fragmentEmptyBlocks_countOrder->reinsertNode(leftMemBlockAny->emptyMemBlockOffsetSorted->nodeByCount);
				}
			}
		}
		if (!isCombinedInNeightbour) {
			// No neighbours are empty fragments, Convert to a empty fragment.
			TreeAVL<EmptyMemBlockOffsetSorted>::Node* emptyMemBlockOffsetSortedNode = fragmentEmptyBlocks_offsetOrder.insertEmplace(memBlockAny);
			EmptyMemBlockOffsetSorted* emptyMemBlockOffsetSorted = &emptyMemBlockOffsetSortedNode->data;

			emptyMemBlockOffsetSorted->nodeByOffset = emptyMemBlockOffsetSortedNode;

			if (fragmentEmptyBlocks_countOrder != nullptr) {
				emptyMemBlockOffsetSorted->nodeByCount = fragmentEmptyBlocks_countOrder->insertEmplace(emptyMemBlockOffsetSorted);
			}

			memBlockAny->emptyMemBlockOffsetSorted = emptyMemBlockOffsetSorted;
		} else {
			// Entry got combined, remove it.
			memBlockAny->selfCEntry->remove();
		}
	}

	syncFillRate();
}

void BMM_Allocator::syncFillRate() {
	float new_fillRateCount = 0;

	if (allocationInsertionType == AllocationInsertionType::EndInsertionBlockOnly) {
		new_fillRateCount = endInsertionBlock_entriesOffset;
	} else if (allocationInsertionType == AllocationInsertionType::FirstRegionMustHaveCapacity || allocationInsertionType == AllocationInsertionType::FirstAvailableSizeCheckedRegion) {
		// Remove the fragments count because they are allocatable free space for this insertion mode.

		new_fillRateCount = endInsertionBlock_entriesOffset - totalFragmentedEntriesCount;
	} else {
		throw LogicException(LOC);
	}

	if (new_fillRateCount != fillRateCount) {
		fillRateCount = new_fillRateCount;
	}
}

BMM_Allocator::~BMM_Allocator() {
	//void
}
