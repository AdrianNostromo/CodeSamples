#pragma once

#include <base/gh.h>
#include "GLBCTBase.h"
#include <base/container/TreeAVL.h>

namespace graphics {
class GLBCTIndividualRegions : public GLBCTBase {priv typedef GLBCTBase super;pub dCtor(GLBCTIndividualRegions);
    prot class EntriesBlock {
        //asd_01;// use a blockEntriesRange:Range1DInt.
        pub int blockEntriesOffset;
        pub int blockEntriesCount;

        pub explicit EntriesBlock(int blockEntriesOffset, int blockEntriesCount);

        pub bool operator==(EntriesBlock const& other) noexcept;
        pub bool operator!=(EntriesBlock const& other) noexcept;
        pub bool operator>(EntriesBlock const& other) noexcept;
        pub bool operator<(EntriesBlock const& other) noexcept;
    };

    prot struct ComparatorOverlappOrContactNeighbour : public ICustomComparator1<EntriesBlock&/*ArgumentType*/, int/*ResultType*/> {
        int targetBlockEntriesOffset = -1;
        int targetBlockEntriesCount = -1;

        int operator()(EntriesBlock& o) const;
    };

    priv TreeAVL<EntriesBlock> invalidEntryRegionsTree{};

    // This is used to find a entry that is connected to a entries_block.
    /// This is used when releasing a entries_block to find a combine location.
    priv ComparatorOverlappOrContactNeighbour comparatorOverlappOrContactNeighbour{};

    pub explicit GLBCTIndividualRegions(CompositesBuffer* buffer);

    //asd_01;// make sure this is cleared when the on_load_to_gl upload occurs;

    pub void validate(GLenum glBindTarget) final;

    pub void invalidateUsedRegion() final;
    pub void invalidateRegion(int blockEntriesOffset, int blockEntriesCount) final;

    pub ~GLBCTIndividualRegions() override;
};
};
