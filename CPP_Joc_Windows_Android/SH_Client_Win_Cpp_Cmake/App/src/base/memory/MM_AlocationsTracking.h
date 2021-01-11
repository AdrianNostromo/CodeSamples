#pragma once

#include <base/gh.h>
#include "MM_Stats.h"
#include <base/container/TreeAVLKeyValue.h>
#include <utility>

namespace mem {
class MM_AlocationsTracking : public MM_Stats {priv typedef MM_Stats super;pub dCtor(MM_AlocationsTracking);
    priv struct LocationIndicator {pub dCtor(LocationIndicator);
        pub const char* fileName;
        pub size_t lineIndex;

        pub size_t allocationsCount = 0;
        pub size_t memLeaksDetectionSnapshot_allocationsCount = 0;

        pub explicit LocationIndicator(const char* fileName, size_t lineIndex)
            : fileName(fileName), lineIndex(lineIndex)
        {}
    };
    priv struct AllocationInformation {pub dCtor(AllocationInformation);
        pub void* blockPtr;
        pub size_t bytesCount;

        pub char allocationType;

        pub TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator>::Node* allocationSource;

        pub explicit AllocationInformation(void* blockPtr, size_t bytesCount, char allocationType, TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator>::Node* allocationSource)
            : blockPtr(blockPtr), bytesCount(bytesCount), allocationType(allocationType), allocationSource(allocationSource)
        {}
    };

    priv TreeAVLKeyValue<void*, AllocationInformation> allocationsTree{ true/*bypassMemoryManagement*/ };
    priv TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator> locationIndicatorsTree{ true/*bypassMemoryManagement*/ };

    priv std::pair<const char*, size_t> t_locationKey;

    pub explicit MM_AlocationsTracking();

    prot void trackAllocation(void* ptr, unsigned int byteCount, char allocationType, const char* fileName, size_t lineIndex);
    // Returns the fileName if any, it is used for errors detection.
    prot const char* untrackAllocation(void* block, char allocationType);

    pub void startDeltaAllocationsSnapshot() final;
    pub void logLargestDeltaAllocations() final;

    pub ~MM_AlocationsTracking() override;
};
};
