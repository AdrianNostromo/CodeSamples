#pragma once

#include <base/gh.h>
#include <cstddef>

namespace mem {
    class MemoryStats;
};

namespace mem {
class IMemoryManager {
    pub class AllocationType {
        pub static const char Void_Pointer;
        pub static const char Object_Single;
        pub static const char Object_Array;
    };
    
    pub virtual void* malloc(unsigned int byteCount, const char* fileName=nullptr, size_t lineIndex=0, char allocationType=AllocationType::Void_Pointer) = 0;
    pub virtual void* realloc(void* block, unsigned int byteCount, const char* fileName=nullptr, size_t lineIndex=0, char allocationType=AllocationType::Void_Pointer) = 0;
    pub virtual void free(void* block, char allocationType=AllocationType::Void_Pointer) = 0;

    pub virtual MemoryStats* getStats() = 0;

    pub virtual void startDeltaAllocationsSnapshot() = 0;
    pub virtual void logLargestDeltaAllocations() = 0;

    pub virtual ~IMemoryManager() = default;
};
};
