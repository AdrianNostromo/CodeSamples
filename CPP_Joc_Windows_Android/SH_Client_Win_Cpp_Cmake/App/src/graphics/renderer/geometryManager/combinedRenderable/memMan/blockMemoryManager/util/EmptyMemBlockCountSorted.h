#pragma once

#include <base/gh.h>

namespace graphics {
    class EmptyMemBlockOffsetSorted;
};

namespace graphics {
class EmptyMemBlockCountSorted {pub dCtor(EmptyMemBlockCountSorted);
    pub class ComparatorCountKey {pub dCtor(ComparatorCountKey);
        // There is no need for an offste comparisson because the find algo will try to find the min valid node.
        pub int blockEntriesCount;

        pub explicit ComparatorCountKey(int blockEntriesCount)
            : blockEntriesCount(blockEntriesCount)
        {};

    };

    pub EmptyMemBlockOffsetSorted* mainBlock;

    pub explicit EmptyMemBlockCountSorted(EmptyMemBlockOffsetSorted* mainBlock);

    pub bool operator==(const EmptyMemBlockCountSorted& other) const noexcept;
    pub bool operator!=(const EmptyMemBlockCountSorted& other) const noexcept;
    pub bool operator>(const EmptyMemBlockCountSorted& other) const noexcept;
    pub bool operator<(const EmptyMemBlockCountSorted& other) const noexcept;

    pub bool operator==(const ComparatorCountKey& other) const noexcept;
    pub bool operator!=(const ComparatorCountKey& other) const noexcept;
    pub bool operator>(const ComparatorCountKey& other) const noexcept;
    pub bool operator<(const ComparatorCountKey& other) const noexcept;
    pub bool operator>=(const ComparatorCountKey& other) const noexcept;
    pub bool operator<=(const ComparatorCountKey& other) const noexcept;

    pub virtual ~EmptyMemBlockCountSorted();
};
};
