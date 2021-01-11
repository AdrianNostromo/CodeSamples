#include "BMM_Base.h"
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/defragmenter/IDefragmenterBlockMemory.h>

using namespace graphics;

BMM_Base::BMM_Base(
	GeometryBufferManager* geometryBufferManager,
	AllocationInsertionType allocationInsertionType, bool trackEmptyFragmentsByCountSorting, int stride,
	int totalEntriesCapacity, bool usesRightToLeftVolatileMemory,
	IDefragmenterBlockMemory* defragmenter)
	: geometryBufferManager(geometryBufferManager),
	allocationInsertionType(allocationInsertionType), trackEmptyFragmentsByCountSorting(trackEmptyFragmentsByCountSorting), stride(stride),
	totalEntriesCapacity(totalEntriesCapacity), usesRightToLeftVolatileMemory(usesRightToLeftVolatileMemory),
	defragmenter(defragmenter) {
	//void
}

BMM_Base::~BMM_Base() {
	if (defragmenter != nullptr) {
		delete defragmenter;
		defragmenter = nullptr;
	}
}
