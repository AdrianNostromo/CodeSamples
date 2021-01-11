#pragma once

#include <base/gh.h>

class IListEntry;
namespace graphics {
    class ICombinedRenderable;
    class AllocatedMemBlock;
};

namespace graphics {
class InCombinedRenderableBlock {pub dCtor(InCombinedRenderableBlock);
    pub void* rawSubclassPointer = nullptr;
    
    pub AllocatedMemBlock* memBlock = nullptr;

    // This is the number of Entry_GeometryManagerBucket that use this.
    // This is separate from the above combinedRenderable value (combinedRenderable can be nullptr).
    // This is used to detect when to remove vertices that are not used in a CombinedRenderable.
    // This is mostly needed for transforms but vertices and indices also use it for some error detection.
    pub int unusedDetectionUseCount = 0;

    // Make class not instantiable.
    prot explicit InCombinedRenderableBlock();

    pub virtual ~InCombinedRenderableBlock();
};
};
