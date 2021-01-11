#pragma once

#include <base/gh.h>
#include "InCombinedRenderableBlock.h"

namespace graphics {
    class GeometryProviderInstance;
};

namespace graphics {
class ICRB_ModelTransforms : public InCombinedRenderableBlock {priv typedef InCombinedRenderableBlock super;pub dCtor(ICRB_ModelTransforms);
    pub class ToRemoveUnusedEntry {pub dCtor(ToRemoveUnusedEntry);
        pub GeometryProviderInstance* gpi_ModelTransforms;
        pub ICRB_ModelTransforms* icrb_modelTransforms;
        pub int cRendUIndex;

        pub explicit ToRemoveUnusedEntry(GeometryProviderInstance* gpi_ModelTransforms, ICRB_ModelTransforms* icrb_modelTransforms, int cRendUIndex)
            : gpi_ModelTransforms(gpi_ModelTransforms), icrb_modelTransforms(icrb_modelTransforms), cRendUIndex(cRendUIndex)
        {
            //void
        }
    };

    pub AllocatedMemBlock* memBlock_transformsIndexRemaping = nullptr;

    // This is used to remove vertices that are no longer used. The remove occurs after the gMan entries are processed.
    pub IListEntry* inGBucket_toRemoveUnused_listEntry = nullptr;

    pub explicit ICRB_ModelTransforms();

    pub static InCombinedRenderableBlock* GetNew();

    pub ~ICRB_ModelTransforms() override;
};
};
