#include "GeometryBufferManager.h"
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>
#include <graphics/geometry/data/BufferObject.h>
#include "../ICombinedRenderable.h"
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/data/UniformBufferObject.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AllocatedMemBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/defragmenter/IDefragmenterBlockMemory.h>

using namespace graphics;

GeometryBufferManager::GeometryBufferManager(
	ICombinedRenderable* combinedRenderable,
	BufferObject* bufferObject, BlockMemoryManager::AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting,
	int vaByteOffset_worldTransformIndex, BufferObject* bufferObjectB,
	int entriesPerGLEntry,
	bool usesStableMemory, bool usesRightToLeftVolatileMemory,
	IDefragmenterBlockMemory* defragmenter_volatile, IDefragmenterBlockMemory* defragmenter_stable)
    : combinedRenderable(combinedRenderable),
	bufferObject(bufferObject),
	vaByteOffset_worldTransformIndex(vaByteOffset_worldTransformIndex), bufferObjectB(bufferObjectB),
	entriesPerGLEntry(entriesPerGLEntry)
{
    data = static_cast<char*>(bufferObject->getBuffer().getData());

    stride = bufferObject->getBuffer().stride();

	// Entries are added and removed as used to avoid larger than needed update on_load_to_gl;
    // bufferObject->getBuffer().appendUninitialised(this->bufferObject->getBuffer().capacity());

	if (bufferObjectB != nullptr) {
		dataB = bufferObjectB->getBuffer().getData();
		strideB = bufferObjectB->getBuffer().stride();

		// Entries are added and removed as used to avoid larger than needed update on_load_to_gl;
		bufferObjectB->getBuffer().appendUninitialised(this->bufferObjectB->getBuffer().capacity());
	}

	blockMM_volatile = new BlockMemoryManager(
		this/*geometryBufferManager*/,
		allocationInsertionType, trackEmptyFragmentsByCountSorting, stride,
		this->bufferObject->getBuffer().capacity() * entriesPerGLEntry/*endInsertionBlock_availableEntriesCount*/, this->bufferObject->getBuffer().capacity() * entriesPerGLEntry/*totalEntriesCapacity*/, usesRightToLeftVolatileMemory,
		defragmenter_volatile
	);

	if (usesStableMemory) {
		blockMM_stable = new BlockMemoryManager(
			this/*geometryBufferManager*/,
			allocationInsertionType, trackEmptyFragmentsByCountSorting, stride,
			this->bufferObject->getBuffer().capacity() * entriesPerGLEntry/*endInsertionBlock_availableEntriesCount*/, this->bufferObject->getBuffer().capacity() * entriesPerGLEntry/*totalEntriesCapacity*/, false,
			defragmenter_stable
		);
	}

	if (usesStableMemory) {
		if (!blockMM_volatile->usesRightToLeftVolatileMemory) {
			// Volatile mem must be rightToLeft when 2 mem regions are used.
			throw LogicException(LOC);
		}

		// Two memory regions are used and the volatile will start at the right. Count=caacity always in this case and the center unused region will not be updated.
		bufferObject->getBuffer().setCount(bufferObject->getBuffer().capacity());

		// Make the entire buffer unused center region. It will be updated as data changes.
		bufferObject->getBuffer().getUnusedCenterRegionRef().set(0, bufferObject->getBuffer().count());
	}
}

int GeometryBufferManager::getIndicesCount() {
	if (blockMM_stable != nullptr) {
		// Indices don't use blockMM_stable;
		throw LogicException(LOC);
	}

	return blockMM_volatile->getIndicesCount();
}

VertexBufferObject* GeometryBufferManager::getBufferObject_asVBO() {
	return static_cast<VertexBufferObject*>(bufferObject->rawSubclassPointer);
}

IndexBufferObject* GeometryBufferManager::getBufferObject_asIBO() {
	return static_cast<IndexBufferObject*>(bufferObject->rawSubclassPointer);
}

UniformBufferObject* GeometryBufferManager::getBufferObject_asUBO() {
	return static_cast<UniformBufferObject*>(bufferObject->rawSubclassPointer);
}

UniformBufferObject* GeometryBufferManager::getBufferObjectB_asUBO() {
	if (bufferObjectB == nullptr) {
		return nullptr;
	}

	return static_cast<UniformBufferObject*>(bufferObjectB->rawSubclassPointer);
}

int GeometryBufferManager::getInsertEntrisCapacityWithAllocationInsertionType() {
	int val = blockMM_volatile->getInsertEntrisCapacityWithAllocationInsertionType();
	
	// Note. blockMM_stable is not uses for insert capacity because it is not allocatable memory (Only defragment code can allocate from it).
	/// Checking only blockMM_volatile for isnert capacity is correct.

	return val;
}

int GeometryBufferManager::getTotalEntriesCapacity() {
	int val = blockMM_volatile->totalEntriesCapacity;

	//asd_01;// for now, don't count blockMM_stable but do some better logic after stable mem is working.

	return val;
}

int GeometryBufferManager::getTotalFragmentedEntriesCount() {
	int val = blockMM_volatile->totalFragmentedEntriesCount;

	if (blockMM_stable != nullptr) {
		val += blockMM_stable->totalFragmentedEntriesCount;
	}

	return val;
}

void GeometryBufferManager::onBlockMMEndInsertionBlockOffsetChanged() {
	if (blockMM_stable != nullptr) {
		// Two region mem management is used.
		if (!blockMM_volatile->usesRightToLeftVolatileMemory) {
			// The volatile mem must be rightToLeft
			throw LogicException(LOC);
		}
		
		int newEndInsertionBLockCapacity = blockMM_volatile->totalEntriesCapacity - (blockMM_stable->endInsertionBlock_entriesOffset + blockMM_volatile->endInsertionBlock_entriesOffset);
		if (newEndInsertionBLockCapacity < 0) {
			// This should never occur.
			throw LogicException(LOC);
		}

		blockMM_stable->endInsertionBlock_availableEntriesCount = newEndInsertionBLockCapacity;
		blockMM_volatile->endInsertionBlock_availableEntriesCount = newEndInsertionBLockCapacity;

		bufferObject->getBuffer().getUnusedCenterRegionRef().set(
			blockMM_stable->endInsertionBlock_entriesOffset, 
			blockMM_stable->endInsertionBlock_entriesOffset + blockMM_stable->endInsertionBlock_availableEntriesCount
		);
	} else {
		// Only left volatile memory is used.
		if (blockMM_volatile->usesRightToLeftVolatileMemory) {
			// The volatile mem must be left
			throw LogicException(LOC);
		}

		int newEndInsertionBLockCapacity = blockMM_volatile->totalEntriesCapacity - blockMM_volatile->endInsertionBlock_entriesOffset;
		blockMM_volatile->endInsertionBlock_availableEntriesCount = newEndInsertionBLockCapacity;
	}
}

AllocatedMemBlock* GeometryBufferManager::allocate(int blockEntriesCount, Entry_GeometryManagerBucket* dataSource_bucketEntry, GeometryProviderInstance* dataSource_gpi_forMt, ICRB_ModelTransforms* dataSource_icrb_forRemapingMt) {
	int oldBlockFillRateCount = blockMM_volatile->fillRateCount;
	int old_endInsertionBlock_entriesOffset = blockMM_volatile->endInsertionBlock_entriesOffset;

	AllocatedMemBlock* val = blockMM_volatile->allocate(blockEntriesCount, dataSource_bucketEntry, dataSource_gpi_forMt, dataSource_icrb_forRemapingMt);

	if (blockMM_volatile->endInsertionBlock_entriesOffset != old_endInsertionBlock_entriesOffset
		&& blockMM_stable == nullptr)
	{
		if (blockMM_volatile->usesRightToLeftVolatileMemory) {
			// Stable memory is used so the volatile must be rightToLeft.
			throw LogicException(LOC);
		}

		bufferObject->getBuffer().setCount(Math::ceil(((float)blockMM_volatile->endInsertionBlock_entriesOffset) / ((float)entriesPerGLEntry)));
	}
	if (oldBlockFillRateCount != blockMM_volatile->fillRateCount) {
		fillRateCount += (blockMM_volatile->fillRateCount - oldBlockFillRateCount);
		fillRatePercent = ((float)fillRateCount) / (bufferObject->getBuffer().capacity() * entriesPerGLEntry);

		combinedRenderable->onMemManFillRateCHanged();
	}

	return val;
}

std::nullptr_t GeometryBufferManager::deallocate(AllocatedMemBlock* memBlock) {
	int oldBlockFillRateCount = blockMM_volatile->fillRateCount;
	int old_endInsertionBlock_entriesOffset = blockMM_volatile->endInsertionBlock_entriesOffset;

	std::nullptr_t val = blockMM_volatile->deallocate(memBlock);

	if (blockMM_volatile->endInsertionBlock_entriesOffset != old_endInsertionBlock_entriesOffset
		&& blockMM_stable == nullptr)
	{
		if (blockMM_volatile->usesRightToLeftVolatileMemory) {
			// Stable memory is used so the volatile must be rightToLeft.
			throw LogicException(LOC);
		}

		// The buffer count is affected by entriesPerGLEntry;
		bufferObject->getBuffer().setCount(Math::ceil(((float)blockMM_volatile->endInsertionBlock_entriesOffset) / ((float)entriesPerGLEntry)));
	}
	if (oldBlockFillRateCount != blockMM_volatile->fillRateCount) {
		fillRateCount += (blockMM_volatile->fillRateCount - oldBlockFillRateCount);
		fillRatePercent = ((float)fillRateCount) / (bufferObject->getBuffer().capacity() * entriesPerGLEntry);

		combinedRenderable->onMemManFillRateCHanged();
	}

	return val;
}

void GeometryBufferManager::tickDefrag() {
	if (blockMM_volatile->defragmenter != nullptr) {
		blockMM_volatile->defragmenter->tickDefrag();
	}
	if (blockMM_stable != nullptr && blockMM_stable->defragmenter != nullptr) {
		blockMM_stable->defragmenter->tickDefrag();
	}
}

GeometryBufferManager::~GeometryBufferManager() {
	if (blockMM_stable != nullptr) {
		delete blockMM_stable;
		blockMM_stable = nullptr;
	}
	if (blockMM_volatile != nullptr) {
		delete blockMM_volatile;
		blockMM_volatile = nullptr;
	}
	if (bufferObject != nullptr) {
		bufferObject->reservedDispose();
		delete bufferObject;
		bufferObject = nullptr;
	}
}
