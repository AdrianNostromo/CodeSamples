#pragma once

#include <base/gh.h>
#include "MM_AlocationsTracking.h"
#include <base/container/TreeAVL.h>
#include <base/list/ListDL.h>
#include <mutex>

namespace base {
    class DebugEntry;
};

namespace mem {
class MemoryManager : public MM_AlocationsTracking { priv typedef MM_AlocationsTracking super; pub dCtor(MemoryManager);
//asd_1;// use the correct typ class;
    priv TreeAVL<int> allBlocksTreeByAddress{};
    priv TreeAVL<int> availableBlocksTreeBySize{};
    priv TreeAVL<int> availableBlocksTreeByAddress{};

    std::mutex mtx{};

    pub explicit MemoryManager();

    pub void* malloc(unsigned int byteCount, const char* fileName=nullptr, size_t lineIndex=0, char allocationType=AllocationType::Void_Pointer) final;
    pub void* realloc(void* block, unsigned int byteCount, const char* fileName=nullptr, size_t lineIndex=0, char allocationType=AllocationType::Void_Pointer) final;
    pub void free(void* block, char allocationType=AllocationType::Void_Pointer) final;

    pub ~MemoryManager() override;
};
};
