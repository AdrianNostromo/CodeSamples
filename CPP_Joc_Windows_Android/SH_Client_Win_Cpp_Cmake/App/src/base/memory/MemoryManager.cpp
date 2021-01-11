#include "MemoryManager.h"
#include <base/MM.h>
#include <cstdlib>

#include <cstdio>
#include <cstdlib>
#include <new>
#include <iostream>

using namespace mem;

MemoryManager::MemoryManager()
    : super()
{
    //void
}

void* MemoryManager::malloc(unsigned int byteCount, const char* fileName, size_t lineIndex, char allocationType) {
    mtx.lock();

    void* ptr = std::malloc(byteCount);
    if (ptr == nullptr || byteCount > 200*1024*1024/*100MB*/) {
        // The size check is to avoid allocating a negative byte count int converted to uint.
        throw std::bad_alloc{};
    }

    // Add to tracking.
    trackAllocation(ptr, byteCount, allocationType, fileName, lineIndex);

    if (allocationType == AllocationType::Void_Pointer) {
        //void*
        stats.functionCallsCount_malloc++;
        if (stats.functionCallsCount_malloc > 1000000000) {
            stats.functionCallsCount_malloc = 0;
        }
        stats.cumulativeByteChanges_malloc++;
        if (stats.cumulativeByteChanges_malloc > 1000000000) {
            stats.cumulativeByteChanges_malloc = 0;
        }

        stats.activesCount_voidPointers++;
    } else {
        // Object
        // Note. Do the stats tracking here because it is part of this class and new,delete don't have access to it.
        stats.functionCallsCount_new++;
        if (stats.functionCallsCount_new > 1000000000) {
            stats.functionCallsCount_new = 0;
        }
        stats.cumulativeByteChanges_new++;
        if (stats.cumulativeByteChanges_new > 1000000000) {
            stats.cumulativeByteChanges_new = 0;
        }

        stats.activesCount_objects++;
    }

    mtx.unlock();

    return ptr;
}

void* MemoryManager::realloc(void* block, unsigned int byteCount, const char* fileName, size_t lineIndex, char allocationType) {
    mtx.lock();

    // Remove from tracking if entry found.
    untrackAllocation(block, allocationType);

    void* ptr = std::realloc(block, byteCount);
    if (ptr == nullptr) {
        throw std::bad_alloc{};
    }

    // Add to tracking.
    trackAllocation(ptr, byteCount, allocationType, fileName, lineIndex);

    stats.functionCallsCount_realloc++;
    if (stats.functionCallsCount_realloc > 1000000000) {
        stats.functionCallsCount_realloc = 0;
    }

    //asd_x;// There is no information until the custom mManager is implemented
    /*stats.cumulativeByteChanges_realoc = stats.cumulativeByteChanges_realoc  - asd + asd;
    if (stats.cumulativeByteChanges_realoc > 1000000000) {
        stats.cumulativeByteChanges_realoc = 0;
    }*/

    mtx.unlock();

    return ptr;
}

void MemoryManager::free(void* block, char allocationType) {
    mtx.lock();

    // Remove from tracking if entry found.
    const char* fileName = untrackAllocation(block, allocationType);

	std::free(block);

    if (allocationType == AllocationType::Void_Pointer) {
        //void*
        stats.functionCallsCount_free++;
        if (stats.functionCallsCount_free > 1000000000) {
            stats.functionCallsCount_free = 0;
        }

        //asd_x;// There is no information until the custom mManager is implemented
        /*stats.cumulativeByteChanges_free += asd;
        if (stats.cumulativeByteChanges_free > 1000000000) {
            stats.cumulativeByteChanges_free = 0;
        }*/

        stats.activesCount_voidPointers--;
    } else {
        // Object
        // Note. Do the stats tracking here because it is part of this class and new,delete don't have access to it.
        stats.functionCallsCount_delete++;
        if (stats.functionCallsCount_delete > 1000000000) {
            stats.functionCallsCount_delete = 0;
        }
        //asd_x;// There is no information until the custom mManager is implemented
        /*stats.cumulativeByteChanges_new++;
        if (stats.cumulativeByteChanges_new > 1000000000) {
            stats.cumulativeByteChanges_new = 0;
        }*/

        stats.activesCount_objects--;
    }

    mtx.unlock();
}

MemoryManager::~MemoryManager() {
	//void
}
