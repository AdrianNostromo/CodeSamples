#pragma once

#include <base/gh.h>
#include <base/container/TreeAVL.h>

namespace graphics {
    class AnyMemBlock;
    class EmptyMemBlockOffsetSorted;
    class EmptyMemBlockCountSorted;
};

namespace graphics {
class EmptyMemBlockOffsetSorted {pub dCtor(EmptyMemBlockOffsetSorted);
    // There is no offset, count here, use the memBlockAny values.
    pub AnyMemBlock* memBlockAny = nullptr;

    pub TreeAVL<EmptyMemBlockOffsetSorted>::Node* nodeByOffset = nullptr;
    pub TreeAVL<EmptyMemBlockCountSorted>::Node* nodeByCount = nullptr;

    pub explicit EmptyMemBlockOffsetSorted(AnyMemBlock* memBlockAny);

    pub dOpsDec(EmptyMemBlockOffsetSorted);

    pub virtual ~EmptyMemBlockOffsetSorted();
};
};
